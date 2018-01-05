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
#ifndef IMPLICITPOITAGGER_H
#define IMPLICITPOITAGGER_H

// hoot
#include <hoot/rnd/visitors/ImplicitTaggerBase.h>

namespace hoot
{

/**
 * Adds tags implicitly derived from POI names to POIs
 *
 * Relevant configuration options are those beginning with implicit.tagger.poi.* in
 * conf/core/ConfigOptions.asciidoc.
 */
class ImplicitPoiTagger : public ImplicitTaggerBase
{
public:

  static std::string className() { return "hoot::ImplicitPoiTagger"; }

  ImplicitPoiTagger();
  ImplicitPoiTagger(const QString databasePath);

protected:

  virtual bool _visitElement(const ElementPtr& e);

};

}

#endif // IMPLICITPOITAGGER_H