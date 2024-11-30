package edu.whu.hyk;

import static org.junit.Assert.assertTrue;

import edu.whu.hytra.EngineFactory;
import edu.whu.hytra.EngineParam;
import org.junit.Before;
import org.junit.Test;

import java.util.HashMap;

/**
 * Unit test for simple App.
 */
public class AppTest {
    public EngineFactory engineFactory;

    /**
     * 初始化引擎
     */
    public AppTest() {
        // NYC 的配置信息
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
}
