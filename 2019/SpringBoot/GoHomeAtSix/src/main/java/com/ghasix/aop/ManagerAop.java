package com.ghasix.aop;

import com.ghasix.util.LogUtil;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Aspect
@Component
public class ManagerAop {

    private final Logger logger = LoggerFactory.getLogger(ManagerAop.class);

    @Around("execution(* com.ghasix.manager..*.*(..))") // 매니저 AoP
    public Object aroundManager(ProceedingJoinPoint pjp) {
        String oldLogLayerStr = LogUtil.changeLogLayer(LogUtil.LAYER_MGR);
        boolean mgrResult = true;
        Object mgrReturn = null;

        try {
            // 매니저 로직 수행   
            try {
                mgrReturn = pjp.proceed();
            }
            catch (Throwable t) {
                logger.error("Manager Exception!", t);
                mgrResult = false;
            }
        }
        catch (Throwable t) {
            logger.error("Manager AoP Exception!", t);
            mgrResult = false;
        }

        // 로그 레이어 되돌리기 및 결과 반환
        LogUtil.changeLogLayer(oldLogLayerStr);
        return mgrReturn;
    }
}