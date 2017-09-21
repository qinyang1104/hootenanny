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
 * @copyright Copyright (C) 22017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SqlBulkUpdate.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QStringList>

// tgs
#include <tgs/System/Time.h>

namespace hoot
{

SqlBulkUpdate::SqlBulkUpdate(QSqlDatabase& db, const QString tableName, const QStringList columns) :
_time(0),
_tableName(tableName),
_columns(columns),
_db(db)
{
  QString sql ="UPDATE " + _tableName + " SET ";
  for (int i = 0; i < _columns.size(); i++)
  {
    sql += columns.at(i) + "= ?, ";
  }
  sql.chop(2);
  sql += " WHERE ID = ?";
  LOG_VART(sql);
  _query.reset(new QSqlQuery(_db));
  if (!_query->prepare(sql))
  {
    LOG_ERROR("Unable to prepare query: " << sql);
  }
  LOG_VART(_query->executedQuery());

  _initValsList();
}

SqlBulkUpdate::~SqlBulkUpdate()
{
  LOG_DEBUG("(" << _tableName << ") Total time updating: " << _time);
  if (_pendingIds.size() > 0)
  {
    LOG_WARN("(" << _tableName << ") There are pending updates in SqlBulkUpdate. You should call "
             "flush before destruction.");
  }
  _query.reset();
}

void SqlBulkUpdate::_initValsList()
{
  for (int i = 0; i < _columns.size(); i++)
  {
    _pendingVals.append(QVariantList());
  }
}

void SqlBulkUpdate::flush()
{
  LOG_TRACE("Flushing bulk update...");
  LOG_VART(_pendingIds.size());
  LOG_VART(_pendingVals.size());

  if (_pendingIds.size() > 0)
  {
    double start = Tgs::Time::getTime();

    assert(_pendingIds.size() == _pendingVals.at(0).size());
    for (int i = 0; i < _pendingVals.size(); i++)
    {
      LOG_VART(_pendingVals.at(i));
      LOG_VART(_pendingVals.at(i).size());
      _query->addBindValue(_pendingVals.at(i));
    }
    LOG_VART(_pendingIds);
    _query->addBindValue(_pendingIds);
    LOG_TRACE(_query->lastQuery());

    if (_query->execBatch() == false)
    {
      LOG_ERROR(_query->executedQuery().left(500));
      LOG_ERROR(_query->lastError().text().left(500));
      throw HootException(
        QString("Error executing bulk insert: %1 (%2)")
        .arg(_query->lastError().text().left(500)).arg(_query->executedQuery()).left(500));
    }
    _query->finish();

    _pendingIds.clear();
    _pendingVals.clear();
    _initValsList();
    double elapsed = Tgs::Time::getTime() - start;
    _time += elapsed;
  }
}

void SqlBulkUpdate::update(const long id, const QVariantList vals)
{
  LOG_VART(_columns.size());
  LOG_VART(vals.size());
  LOG_VART(vals);
  LOG_VART(_pendingIds.size());
  LOG_VART(_pendingVals.size());

  if (vals.size() != _columns.size())
  {
    LOG_VART(_columns);
    throw IllegalArgumentException("Expected vals to have the same size as _columns.");
  }
  _pendingIds.append((qlonglong)id);
  //The vals come in as a complete set for one record, and the batch exec wants to see a separate
  //list for each val type
  for (int i = 0; i < vals.size(); i++)
  {
    _pendingVals[i].append(vals.at(i));
  }
}

}
