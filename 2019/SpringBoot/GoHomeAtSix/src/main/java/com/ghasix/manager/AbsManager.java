package com.ghasix.manager;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public abstract class AbsManager {

    private final static Logger logger = LoggerFactory.getLogger(AbsManager.class);

    protected boolean managerInitialized = false;   // 매니저 초기화 여부
    protected String managerName = null;            // 매니저 이름
    protected String managerVersion = null;         // 매니저 버전
    
    // 매니저 초기화 함수
    public boolean init() {
        logger.error("Do not access AbsManager.init() method!");
        return false;
    }

    // 게터/세터
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
