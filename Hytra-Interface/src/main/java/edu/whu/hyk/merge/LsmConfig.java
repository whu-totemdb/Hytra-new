package edu.whu.hyk.merge;

import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

public class LsmConfig {

    private HashMap<String, String> mergeMap;

    private HashMap<Integer, HashSet<String>> keysPerLevel;

    private List<Integer> elementSizeThresholdPerLevel;

    private Integer elementLengthPerLevel;

    public HashMap<String, String> getMergeMap() {
        return mergeMap;
    }

    public void setMergeMap(HashMap<String, String> mergeMap) {
        this.mergeMap = mergeMap;
    }

    public HashMap<Integer, HashSet<String>> getKeysPerLevel() {
        return keysPerLevel;
    }

    public void setKeysPerLevel(HashMap<Integer, HashSet<String>> keysPerLevel) {
        this.keysPerLevel = keysPerLevel;
    }

    public List<Integer> getElementSizeThresholdPerLevel() {
        return elementSizeThresholdPerLevel;
    }

    public void setElementSizeThresholdPerLevel(List<Integer> elementSizeThresholdPerLevel) {
        this.elementSizeThresholdPerLevel = elementSizeThresholdPerLevel;
    }

    public Integer getElementLengthPerLevel() {
        return elementLengthPerLevel;
    }

    public void setElementLengthPerLevel(Integer elementLengthPerLevel) {
        this.elementLengthPerLevel = elementLengthPerLevel;
    }

    public String saveTo(String filePath, String fileName) throws IOException {
        String fullName = filePath + "/" + fileName;
        File checkFile = new File(fullName);
        if (!checkFile.exists()) {
            checkFile.createNewFile();
        }
        Path path = Paths.get(fullName);

        try (BufferedWriter writer = Files.newBufferedWriter(path, StandardCharsets.UTF_8)) {
            writer.write("merge_map\n");
            for (Map.Entry<String, String> entry : mergeMap.entrySet()) {
                writer.write(entry.getKey());
                writer.write(":");
                writer.write(entry.getValue());
                writer.write("\n");
            }
            writer.write("\n");
            writer.write("keys_per_level\n");
            for (Map.Entry<Integer, HashSet<String>> entry : keysPerLevel.entrySet()) {
                HashSet<String> strings = entry.getValue();
                for(String s : strings) {
                    writer.write(entry.getKey().toString());
                    writer.write(":");
                    writer.write(s);
                    writer.write("\n");
                }

            }
            writer.write("\n");
            writer.write("element_size_threshold_per_level\n");
            for (int i = 0; i < elementSizeThresholdPerLevel.size(); i++) {
                writer.write(String.valueOf(i));
                writer.write(":");
                writer.write(String.valueOf(elementSizeThresholdPerLevel.get(i)));
                writer.write("\n");
            }
            writer.write("\n");
            writer.write("element_length_per_level\n");
            writer.write("all:");
            writer.write(String.valueOf(elementLengthPerLevel));
        }
        return fullName;
    }
}
