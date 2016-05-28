## UI Tests

These functional tests exercise the entire system through the user interface using Cucumber and Capybara.  A separate feature in its own file should be written for each piece of UI functionality to be tested, though each feature contains multiple scenarios that are combined when the UI workflow makes sense.

Hoot on Vagrant is pre-configured for the Cucumber dependencies.  If you're running hoot outside of Vagrant, you'll need to go through the Cucumber dependency setup steps listed in the Developer's Guide.

To run the tests from the `$HOOT_HOME` directory in silent-ish mode:

`make ui-test -s`

To generate the coverage reports using the istanbul tool from the `$HOOT_HOME` directory in silent-ish mode:

`make ui-coverage -s`

Three coverage report folders are generated in the `$HOOT_HOME/hoot-ui` directory:

* mocha
* cucumber
* combined


To run the tests from the local directory:

`make test` 

To generate the coverage reports from the local directory:

`make coverage`

Individual tests can be run from the  local directory like so:

`xvfb-run --server-args="-screen 0, 1024x768x24" cucumber --format progress features/settings.feature`

To measure live code coverage while running tests, steps from the `$HOOT_HOME/scripts/CoverHootUI.sh` script can be used:

1. To instrument the javascript code for istanbul we:

    ```
    cd to hoot-ui
    npm run cover
    ```
    
2. Then we want to deploy this using these lines from the cover script:

    `#move istanbul instrumented hoot-ui javascript code into tomcat webapps`
    
    ```
    sudo -u tomcat6 rm -rf /var/lib/tomcat6/webapps/hootenanny-id/js/hoot
    sudo -u tomcat6 rm -rf /var/lib/tomcat6/webapps/hootenanny-id/js/id
    sudo -u tomcat6 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/istanbul/hoot /var/lib/tomcat6/webapps/hootenanny-id/js/hoot
    sudo -u tomcat6 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/istanbul/id /var/lib/tomcat6/webapps/hootenanny-id/js/id
    ```
    
3. Then run the next set of lines to start the instanbul middleware:

    `#start istanbul-middleware`
    
    ```
    cd $HOOT_HOME/test-files/ui/coverage_app
    node app.js
    ```
    
4. Then you should be able to run individual cucumber tests as described above,
and to gauge coverage on the source files the tests are hitting, open the istanbul middleware url to get the live coverage stats:

    http://localhost:8880/coverage

5. If using Vagrant, you'll want to forward this local port to the vm as a config item in VagrantfileLocal:
    ```
    Vagrant.configure(2) do |config|
      config.vm.network "forwarded_port", guest: 8880, host: 8880
    end
    ```
