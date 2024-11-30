# Transitnet

The back-end of the real-time bus trajectory data visualization platform and also the unified search engine of Hytra

> Please use jdk 11 to compile and run.
## Getting started

### 1. Database configuration

Ensure that the database is configured correctly in application.properties.

### 2. Data Prepare (Optional)

1. `CREATE TABLE` statements are located at `src/main/resources/sql` directory，go to your database and create these tables.

2. GTFS Data: https://transitfeeds.com/p/mta

3. GTFS RealTime Data: http://bt.mta.info/wiki/Developers/Index

4. An offline version of data at Kaggle: https://www.kaggle.com/datasets/haoxingxiao/new-york-city-realtime-bus-data

### 3. Package And Execute

Make a jar package for this project:

``` bash
mvn package
```

Run the java application:
``` bash
 java -jar -Dspring.profiles.active=dev target/transitnet-0.0.1-SNAPSHOT-execute.jar 
```
> The default value for `-Dspring.profiles.active` is `dev`. You only need to indicate this property to `prod` on production environment.
