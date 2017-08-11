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
#include "SparkChangesetWriter.h"

// geos
#include <geos/geom/Envelope.h>

using namespace geos::geom;

// hoot
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>

// Qt
//#include <QStringBuilder> //could optimize with this later, if needed

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangeWriter, SparkChangesetWriter)

SparkChangesetWriter::SparkChangesetWriter() :
_precision(round(ConfigOptions().getWriterPrecision())),
_tmpMap(OsmMapPtr(new OsmMap()))
{

}

SparkChangesetWriter::~SparkChangesetWriter()
{
  close();
}

void SparkChangesetWriter::open(QString fileName)
{
  close();

  _fp.reset(new QFile());
  _fp->setFileName(fileName);
  if (!_fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(fileName));
  }

  // find a match creator that can provide the search bounds.
  foreach (boost::shared_ptr<MatchCreator> mc, MatchFactory::getInstance().getCreators())
  {
    SearchRadiusProviderPtr sbc = boost::dynamic_pointer_cast<SearchRadiusProvider>(mc);
    if (sbc.get())
    {
      if (_bounds.get())
      {
        LOG_WARN("Found more than one bounds calculator. Using the first one.");
      }
      else
      {
        _bounds.reset(new SearchBoundsCalculator(sbc));
      }
    }
  }

  if (!_bounds.get())
  {
    throw HootException("You must specify one match creator that supports search radius "
      "calculation.");
  }
}

void SparkChangesetWriter::writeChange(const Change& change)
{
  if (change.e->getElementType() != ElementType::Node)
  {
    throw NotImplementedException("Only nodes are supported.");
  }

  QString changeType;
  switch (change.type)
  {
    case Change::Create:
      changeType = "A";
      break;
    case Change::Modify:
      changeType = "M";
      break;
    case Change::Delete:
      changeType = "D";
      break;
    default:
      throw IllegalArgumentException("Unexpected change type.");
  }

  ConstNodePtr node = boost::dynamic_pointer_cast<const Node>(change.e);
  NodePtr nodeCopy(dynamic_cast<Node*>(node->clone()));
  _exportTagsVisitor.visit(nodeCopy);
  Envelope env = _bounds->calculateSearchBounds(OsmMapPtr(), nodeCopy);

  QString changeLine;
  changeLine += changeType + "\t";
  changeLine += QString::number(env.getMinX(), 'g', 16) + "\t";
  changeLine += QString::number(env.getMinY(), 'g', 16) + "\t";
  changeLine += QString::number(env.getMaxX(), 'g', 16) + "\t";
  changeLine += QString::number(env.getMaxY(), 'g', 16) + "\t";
  if (change.type == Change::Modify)
  {
    // element hash before change
    if (!change.previousElement.get())
    {
      throw HootException("No previous element specified for modify change.");
    }
    else if (change.previousElement->getElementType() != ElementType::Node)
    {
      throw NotImplementedException("Only nodes are supported.");
    }
    ConstNodePtr previousNode = boost::dynamic_pointer_cast<const Node>(change.previousElement);
    NodePtr previousNodeCopy(dynamic_cast<Node*>(previousNode->clone()));
    _exportTagsVisitor.visit(previousNodeCopy);
    changeLine +=
      QString::fromUtf8(CalculateHashVisitor::toHash(previousNodeCopy).toHex().data()) + "\t";
  }
  // element hash after change
  changeLine += QString::fromUtf8(CalculateHashVisitor::toHash(nodeCopy).toHex().data()) + "\t";
  _tmpMap->clear();
  _tmpMap->addElement(nodeCopy);
  changeLine += _jsonWriter.toString(_tmpMap);
  changeLine += "\n";

  if (_fp->write(changeLine.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _fp->errorString());
  }
}

}