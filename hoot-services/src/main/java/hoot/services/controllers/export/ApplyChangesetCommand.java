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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.OSMAPI_DB_URL;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/*
== apply-changeset

=== Description

The +apply-changeset+ command writes an OSM changeset file that represents the difference
between two OSM files to an OSM API database.  Optional conflict parameters may be
passed, so that if the target database contains any changesets created after the time
represented by a timestamp that intersect with a specified AOI, the command will return
an error.

* +sqlChangeset+ - input SQL changeset (e.g. .osc.sql file).
* +targetDatabaseUrl+ - database the changeset will be written to.
* +conflictAoi+ - bounding box string of the form: minx,miny,maxx,maxy used to prevent writing conflicted data (see description)
* +conflictTimestamp+ - timestamp of the form: "yyyy-MM-dd hh:mm:ss.zzz" used to prevent writing conflicted data (see description)

=== Usage

--------------------------------------
apply-changeset (sqlChangeset) (targetDatabaseUrl) [conflictAoi] [conflictTimestamp]
--------------------------------------

==== Examples

--------------------------------------
hoot apply-changeset changeset.osc.sql osmapidb://username:password@localhost:5432/databaseName

hoot apply-changeset changeset.osc.sql osmapidb://username:password@localhost:5432/databaseName -93.89258,40.96917,-93.60583,41.0425 "2016-05-04 10:15:37.000"-93.89258,40.96917,-93.60583,41.0425
--------------------------------------
 */

public class ApplyChangesetCommand extends ExportCommand {

    ApplyChangesetCommand(String jobId, Map<String, String> paramMap, String debugLevel, Class<?> caller) {
        super(jobId, paramMap, debugLevel, caller);

        String aoi = super.getBoundingBox();
        String output = super.getOutputPath();

        List<String> options = super.getHootOptions();
        options.add("-D convert.bounding.box=" + aoi);
        options.add("-D osm.changeset.sql.file.writer.generate.new.ids=false");
        String hootOptions = options.stream().collect(Collectors.joining(" "));

        String input1 = OSMAPI_DB_URL;
        String input2 = super.getInput();

        //hoot derive-changeset $(HOOT_OPTS) -D convert.bounding.box=$(aoi) -D osm.changeset.sql.writer.generate.new.ids=false $(input1) $(input2) "$(OP_OUTPUT)"
        String command = "hoot derive-changeset --" + debugLevel + " " + hootOptions + " " + input1 + " " + input2 + " " + output;

        super.configureAsHootCommand(command, caller);
    }
}
