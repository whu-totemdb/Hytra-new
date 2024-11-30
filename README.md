# Hytra

<meta name="robots" content="noindex">
A Hybrid and Efficient System for Massive Trajectory Data Management.

## Repository Overview

- Hytra-Interface: the global encoding engine of Hytra.
- Hytra-LSM: the hybrid storage engine of Hytra.
- transitnet: the unified search engine of Hytra, where we implement the real-time range query, historical range query, real-time kNN query, and historical kNN query. It is also the back-end of our designed real-time bus trajectory data visualization platform.
- transitnet-vue: the front-end of the real-time bus trajectory data visualization platform.

## Relationships Between Projects

![image-20230802181152370](https://github.com/TotemSmartBus/Hytra/blob/main/relationships.png)

- Hytra-Interface is a dependency project for the underlying LSM-tree index used by transitnet. It needs to be packaged into a jar file and placed under the lib folder of transitnet.
- Hytra-Interface and Hytra-LSM use sockets to communicate.
- Transitnet provides underlying services to transitnet-vue, and transitnet-vue renders the interface to realize real-time bus trajectory data visualization and support interactive queries for users.

<!--Each project has its own .md file that describes its specific content and use.-->

## Features of Hytra

* Trajectory storage based on an LSM-tree-extended architecture called *Adjacency-based Compaction Tree* (ACT).
  * Key-value store.
  * Adjacency-based compaction policy.
  * Optimized storage architecture.
* Fast trajectory queries with
  * Enriched posting lists.
  * A search operator *SweepLine*.
  * A unified similarity measure *LOGC*.
  * A *shape-trip-schedule* three-level index for bus trajectory data.
* Bus trajectory visualization in New York City

## Supported Queries

Hytra is able to efficiently answer four typical types of queries over trajectory data now:

* Real-time range query
* Historical range query
* Real-time kNN query
* Historical kNN query

## Getting Started

### 0. Preparing Dataset

Use the *.sql* files located in the *transitnet/src/main/resources/sql* folder to create the database and database tables. Then, run the main program of transitnet to collect trajectory data. Additionally, configure the *application-dev.properties* file in the *resources* directory. 

### 1. Dependencies

We manage the dependent libraries with Maven. You can easily install those required software in pom.xml file.

### 2. Running the sample program

We provide a use case for the historical range query. The `ExpTest()` method is in the `HistoricalRangeExpTest_merge` class in the transitnet project.

```java
//1. Set the parameters.     
double []ps={40.8100, -73.9200, 40.8367, -73.8840};
String date="2023-05-20";
historicalrangeExpService.setup(ps,date);

//2. Execute query.
historicalrangeExpService.historaical_range_search();
```

Transitnet provides simple APIs for query processing.

* `historicalrangeExpService.setup()` initializes the query parameters, including the spatial range and the specific date.
* `historicalrangeExpService.historaical_range_search()` encapsulates the process of merging cubes, updating indexes, generating sweeplines, and executing queries.

### Query Types

#### 1) Real-time range query

```java
RealtimeRangeService.setup(temp);
RealtimeRangeService.hytra();
```

The historical range query is used to retrieve trajectories passing through a rectangular area at the current time.

#### 2) Historical range query

```java
historicalrangeExpService.setup(ps,date);
historicalrangeExpService.historaical_range_search();
```

The historical range query is used to retrieve trajectories passing through a rectangular area in a period of time.

#### 3) Real-time kNN query

```java
QueryKnnRtParam param=new QueryKnnRtParam(ps);
realtimeKNNExpService.setup(param.getPoints(),k,backdate);
realtimeKNNExpService.getTopKTrips();
```

The real-time kNN query is used to retrieve the k highest ranked trajectories based on the similarity to the query trajectory. 

#### 4) Historical kNN query

```java
QueryKnnHisParam param=new QueryKnnHisParam(ps,ts);
historicalKNNExpService.setup(param.getPoints(),k);
historicalKNNExpService.getTopKTrips();
```

The historical kNN query is used to retrieve the k highest ranked trajectories based on the similarity to the query trajectory. 

## Future Directions

1. Support other advanced trajectory queries.
2. Develop a distributed version of Hytra for extremely large volumes of trajectory data.

## Citation

If you use our code for research work, please cite our paper as below:
```
@inproceedings{wen2024towards,
  title={Towards Unified Spatio-Temporal Index for Hybrid Trajectory Search},
  author={Tianyao Wen, Shengkun Zhu, Yiming Wang and Sheng Wang},
  booktitle={{ADMA}},
  volume={15389},
  pages={??--??},
  year={2024}
}
```
