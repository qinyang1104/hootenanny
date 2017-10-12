#!/bin/bash
set -e

#TODO: convert this to a regression test

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep derive-poi-implicit-tag-rules`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "multiary-ingest command not present.  Exiting test without running it."
 exit 0
fi

REF_DIR=/home/vagrant/hoot/tmp
OUTPUT_DIR=/home/vagrant/hoot/tmp
#INPUT_DIR=test-files/io/PoiImplicitTagRulesDeriverTest

#rm -rf $OUTPUT_DIR
#mkdir -p $OUTPUT_DIR
rm -f $OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.json
rm -f $OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.tsv
rm -f $OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.sqlite

hoot derive-poi-implicit-tag-rules --warn /home/vagrant/hoot/tmp/yemen-latest-2.osm.pbf "translations/OSM_Ingest.js" "$OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.json;$OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.tsv;$OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.sqlite" "amenity;tourism;building" 2

#only checking the json and tsv files here
diff $REF_DIR/DeriveYemenPoiImplicitTagRulesCmdTest.json $OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.json
diff $REF_DIR/DeriveYemenPoiImplicitTagRulesCmdTest.tsv $OUTPUT_DIR/DeriveYemenPoiImplicitTagRulesCmdTest-out.tsv
 
