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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTATTRIBUTETYPE_H
#define ELEMENTATTRIBUTETYPE_H

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/elements/ElementType.h>

// Qt
#include <QString>

namespace hoot {

/**
 * Represents common element properties that get serialized as XML attributes when writing to file.
 *
 * would like to find a way to make this a little more extensible and less dependent on what's in
 * ElementData...maybe eventually replace this with an enum in ElementData?
 */
class ElementAttributeType
{

public:

  typedef enum Type
  {
    Changeset = 0,
    Timestamp = 1,
    User = 2,
    Uid = 3,
    Version = 4
  } Type;

  ElementAttributeType() { _type = Changeset; }
  ElementAttributeType(Type type) { _type = type; }

  bool operator==(ElementAttributeType t) const { return t._type == _type; }
  bool operator!=(ElementAttributeType t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

private:

  ElementAttributeType::Type _type;

};

}

#endif // ELEMENTATTRIBUTETYPE_H
