# Hytra-Interface

The global encoding engine of Hytra

## Usage
This project is a dependency of the underlying LSM-tree index provided for use by [TransitNet](https://github.com/TotemSmartBus/transitnet). It needs to be packaged as a jar file to be used under the project lib.

1. Pack

``` bash
mvn package
```

2. Move to the Transitnet project

``` bash
cp target/Hytra-Exp-1.0-SNAPSHOT.jar ${TRANSITNET_ROOT}/lib/
```

3. Launch the TransitNet project

> See the README for this project.

## Related Code Samples

>
Excerpt from [RealtimeDataIndex.java from TransitNet](https://github.com/TotemSmartBus/transitnet/blob/master/src/main/java/whu/edu/cs/transitnet/service/index/RealtimeDataIndex.java)
。

```java
// A class is used to manage all index-related methods
public class RealtimeDataIndex {

    // Only PID is returned when the underlying information is queried. The corresponding information needs to be searched by map. Complete information about a hashmap PID and corresponding points is maintained here.
    private LinkedList<HashMap<Integer, Vehicle>> pointToVehicle = new LinkedList<>();

    // Index Engine for the underlying package.
    public EngineFactory engineFactory;

    // Some configuration information is initialized by default.
    public RealtimeDataIndex() {
        EngineParam params = new EngineParam(
                "nyc",
                new double[]{40.502873, -74.252339, 40.93372, -73.701241},
                6,
                "@",
                30,
                (int) 1.2e7
        );
        engineFactory = new EngineFactory(params);
    }

    // Update index
    public void update(List<Vehicle> list) {
        Thread t = new Thread(new IndexUpdater(list));
        t.start();
    }

    // A kNN query is performed on the underlying index, and the parameters passed in are the coordinate point and the value of k.
    public List<Vehicle> search(double lat, double lon, int k) {
        List<Integer> pidList = engineFactory.searchRealtime(lat, lon, k);
        List<Vehicle> result = new ArrayList<>(pidList.size());
        HashMap<Integer, Vehicle> newestMap = pointToVehicle.getLast();
        HashMap<Integer, Vehicle> newestButOneMap = pointToVehicle.get(pointToVehicle.size() - 2);
        // The most recent two time shards are maintained here instead of one, because the data may have been refreshed during the query, in which case the data may be found on the second latest shard.
        for (Integer i : pidList) {
            if (newestMap.containsKey(i)) {
                result.add(newestMap.get(i));
            } else if (newestButOneMap.containsKey(i)) {
                result.add(newestButOneMap.get(i));
            } else {
                log.warn("kNN 搜索到的 PID 索引在最新 2 个时间分片上不存在，是否数据已经更新了？");
            }
        }
        return result;
    }

    // Asynchronous update index.
    class IndexUpdater implements Runnable {
        private List<edu.whu.hytra.entity.Vehicle> newIndex;

        public IndexUpdater(List<Vehicle> newIndex) {
            // You also need to maintain the pid and map of the original data when you update the index.
            HashMap<Integer, Vehicle> pointMap = new HashMap<>();
            this.newIndex = newIndex.stream().map(i -> {
                edu.whu.hytra.entity.Vehicle j = new edu.whu.hytra.entity.Vehicle();
                j.setId(i.getId());
                j.setRecordedTime(i.getRecordedTime());
                j.setLat(i.getLat());
                j.setLon(i.getLon());
                j.setTripID(i.getTripID());
                pointMap.put(j.getPID(), i);
                return j;
            }).collect(Collectors.toList());
            pointToVehicle.add(pointMap);
            // Save only the latest 10 original time shards to prevent excessive memory usage.
            if (pointToVehicle.size() > 10) {
                pointToVehicle.poll();
            }
        }

        @Override
        public void run() {
            engineFactory.updateIndex(newIndex);
        }
    }
}

```
