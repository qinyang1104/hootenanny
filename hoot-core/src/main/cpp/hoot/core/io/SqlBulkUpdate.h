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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SQLBULKUPDATE_H
#define SQLBULKUPDATE_H

// Hoot
#include <hoot/core/io/BulkUpdate.h>

// Qt
#include <QSqlQuery>
#include <QStringList>

class QSqlDatabase;

namespace hoot
{

/**
 * This is designed for batching multiple update SQL operations in a single call.
 */
class SqlBulkUpdate : public BulkUpdate
{

public:

  SqlBulkUpdate(QSqlDatabase& db, const QString tableName, const QStringList columns);

  virtual ~SqlBulkUpdate();

  virtual void flush();

  virtual int getPendingCount() const { return _pendingIds.size(); }

  virtual void update(const long id, const QVariantList vals);

private:

  QVariantList _pendingIds;
  QList<QVariantList> _pendingVals;
  boost::shared_ptr<QSqlQuery> _query;
  double _time;
  QString _tableName;
  QStringList _columns;
  QSqlDatabase _db;

  void _initValsList();
};

}

#endif // SQLBULKUPDATE_H
