/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SqlBulkInsert.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QStringBuilder>

// tgs
#include <tgs/System/Time.h>

namespace hoot
{

SqlBulkInsert::SqlBulkInsert(QSqlDatabase& db, const QString tableName, const QStringList columns) :
_db(db),
_tableName(tableName),
_columns(columns),
_time(0),
_pendingCount(0)
{
  QString sql ="INSERT INTO " + _tableName + "(" + _columns.join(", ")  + ") VALUES (";
  for (int i = 0; i < _columns.size(); i++)
  {
    sql += "?, ";
  }
  sql.chop(2);
  sql += ")";
  LOG_VART(sql);
  _query.reset(new QSqlQuery(_db));
  if (!_query->prepare(sql))
  {
    LOG_ERROR("Unable to prepare query: " << sql);
  }
  LOG_VART(_query->executedQuery());
}

SqlBulkInsert::~SqlBulkInsert()
{
  LOG_DEBUG("(" << _tableName << ") Total time inserting: " << _time);
  if (_pendingCount > 0)
  {
    LOG_WARN(
      "(" << _tableName << ") There are " << _pendingCount << " pending inserts in " <<
      "SqlBulkInsert. You should call flush before destruction.");
  }
  _pending.clear();
  _pendingCount = 0;
}

void SqlBulkInsert::_initValsList()
{
  for (int i = 0; i < _columns.size(); i++)
  {
    _pending.append(QVariantList());
  }
}

void SqlBulkInsert::flush()
{
  LOG_TRACE("Flushing bulk insert for " << _tableName << "...");
  LOG_VART(_pendingCount);

  assert(_pending.size() > 0);
  if (_pendingCount > 0)
  {
    double start = Tgs::Time::getTime();

    for (int i = 0; i < _pending.size(); i++)
    {
      LOG_VART(i);
      LOG_VART(_pending.at(i));
      assert(_pending.at(i).size() == _pendingCount);
      //LOG_VART(_pending.at(i).size());
      _query->addBindValue(_pending.at(i));
    }
    LOG_TRACE(_query->lastQuery());

    if (_query->execBatch() == false)
    {
      _pending.clear();
      _pendingCount = 0;
      LOG_ERROR(_query->executedQuery().left(500));
      LOG_ERROR(_query->lastError().text().left(500));
      throw HootException(
        QString("Error executing bulk insert: %1 (%2)")
        .arg(_query->lastError().text().left(500)).arg(_query->executedQuery()).left(500));
    }
    _query->finish();

    _pending.clear();
    _pendingCount = 0;
    double elapsed = Tgs::Time::getTime() - start;
    _time += elapsed;
  }
}

void SqlBulkInsert::insert(const QVariantList& vals)
{
  if (vals.size() != _columns.size())
  {
    LOG_VAR(vals);
    LOG_VAR(_columns);
    throw IllegalArgumentException("Expected vals to have the same size as _columns.");
  }
  //The vals come in as a complete set for one record, and the batch exec wants to see a separate
  //list for each val type
  if (_pending.size() == 0)
  {
    _initValsList();
  }
  for (int i = 0; i < vals.size(); i++)
  {
    _pending[i].append(vals.at(i));
  }
  _pendingCount++;
}

}
