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
#ifndef SQLBULKINSERT2_H
#define SQLBULKINSERT2_H

// Hoot
#include <hoot/core/io/BulkInsert.h>

// Qt
#include <QSqlQuery>
#include <QStringList>

class QSqlDatabase;

namespace hoot
{

/**
 * This is designed for combining multiple insert SQL operations into a single insert SQL statement.
 * It works with node only datasets.  This may be fixed by #1823.
 *
 * The main difference between this class and SqlBulkInsert is that this classes uses a prepared
 * query that is executed with QSqlQuery::execBatch.  For node writing, this has shown performance
 * improvements when compared to writing with SqlBulkInsert.  However, when writing ways with this
 * class in addition to nodes, way node constraint violations occur complaining that certain nodes
 * do not exist.  Therefore, this class is currently only accessed from node writing only workflows,
 * like Multiary Conflation.
 */
class SqlBulkInsert2 : public BulkInsert
{
public:

  SqlBulkInsert2(QSqlDatabase& db, const QString tableName, const QStringList columns);

  virtual ~SqlBulkInsert2();

  virtual void flush();

  virtual int getPendingCount() const { return _pendingCount; }

  virtual void insert(const QVariantList& vals);

private:

  QList<QVariantList> _pending;
  boost::shared_ptr<QSqlQuery> _query;
  QSqlDatabase _db;
  QString _tableName;
  QStringList _columns;
  double _time;
  int _pendingCount;

  void _initValsList();
};

}

#endif // SQLBULKINSERT2_H
