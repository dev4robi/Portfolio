package com.ghasix.manager;

import javax.annotation.PostConstruct;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Component
public class TidManager extends AbsManager {

    private final Logger logger = LoggerFactory.getLogger(TidManager.class);

    private long accessCnt = 0;

    @PostConstruct
    @Override
    public boolean init() {
        // Name and Version
        managerName     = "TidManager";
        managerVersion  = "1.0.0";
        return true;
    }

    public String generateTid() {
        return String.valueOf(accessCnt++);
    }
}