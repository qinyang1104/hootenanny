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
#include <hoot/core/io/SqlBulkInsert.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QStringBuilder>

// tgs
#include <tgs/System/Time.h>

namespace hoot
{

SqlBulkUpdate::SqlBulkUpdate(QSqlDatabase& db, const QString &tableName, const QStringList& columns) :
_time(0)
{
  QString sql ="UPDATE " + tableName + " SET ";
  for (int i = 0; i < columns.size(); i++)
  {
    sql += columns.at(i) + "= ?, ";
  }
  sql.chop(2);
  sql += " WHERE ID = ?";
  _query.reset(new QSqlQuery(db));
  _query->prepare(sql);
}

SqlBulkUpdate::~SqlBulkUpdate()
{
  //TODO: bring back table name mem var
//  LOG_DEBUG("(" << _tableName << ") Total time updating: " << _time);
//  if (_pending.size() > 0)
//  {
//    LOG_WARN("(" << _tableName << ") There are pending inserts in SqlBulkUpdate. You should call "
//             "flush before destruction.");
//  }
  _query.reset();
}

void SqlBulkUpdate::flush()
{
  for (QList< QMap<long, QList<QVariant> > >::const_iterator it = _pending.begin();
       it != _pending.end(); ++it)
  {

  }
}

void SqlBulkUpdate::update(const long id, const QList<QVariant> l)
{
  QMap<long, QList<QVariant> > idMap;
  idMap[id] = l;
  _pending.append(idMap);
}

}
