package edu.whu.hytra.merge;

import edu.whu.hyk.App;
import edu.whu.hyk.AppTest;
import edu.whu.hyk.DBTest;
import edu.whu.hyk.IndexTest;
import edu.whu.hyk.merge.Generator;
import edu.whu.hyk.merge.LsmConfig;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;

public class lsmConfigTest {

    @Before
    public void prepare() {
        AppTest app = new AppTest();
        DBTest test = new DBTest();
        IndexTest index = new IndexTest();
        index.prepare();
        index.TestUpdateIndex();
    }

    /**
     * 测试是否能正确获取到配置并写入文件
     */
    @Test
    public void generateConfigTest() throws IOException {
        LsmConfig config = Generator.generateConfig();
        String fullName = config.saveTo("/Users/haoxingxiao/Downloads", "test.index");
        Assert.assertTrue(config.getMergeMap().size() > 0);
    }
}
