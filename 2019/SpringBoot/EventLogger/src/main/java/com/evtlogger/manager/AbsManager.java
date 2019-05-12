package com.evtlogger.manager;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public abstract class AbsManager {
    // [Class private constants]
    private final static Logger logger = LoggerFactory.getLogger(AbsManager.class);

    // [Class private variables]
    protected boolean managerInitialized = false;   // 매니저 초기화 여부
    protected String managerName = null;            // 매니저 이름
    protected String managerVersion = null;         // 매니저 버전
    
    // [Methods]
    // 매니저 초기화 함수
    public boolean init() {
        logger.error("Do not access AbsManager.init() method!");
        return false;
    }

    // [Getter/Setters]
    public boolean isManagerInitialized() {
        return managerInitialized;
    }
    public void setManagerInitialized(boolean managerInitialized) {
        this.managerInitialized = managerInitialized;
    }
    public String getManagerName() {
        return managerName;
    }
    public String getManagerVersion() {
        return managerVersion;
    }
}
