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
public class ServiceAop {

    private final Logger logger = LoggerFactory.getLogger(ServiceAop.class);

    @Around("execution(* com.ghasix.service..*.*(..))") // 서비스 AoP
    public Object aroundService(ProceedingJoinPoint pjp) {
        String oldLogLayerStr = LogUtil.changeLogLayer(LogUtil.LAYER_SVC);
        long startTime = System.currentTimeMillis();
        boolean svcResult = true;
        Object svcReturn = null;

        try {
            // 매니저 로직 수행   
            try {
                svcReturn = pjp.proceed();
            }
            catch (Throwable t) {
                logger.error("Service Exception!", t);
                svcResult = false;
            }
        }
        catch (Throwable t) {
            logger.error("Service AoP Exception!", t);
            svcResult = false;
        }

        // 결과 로깅 및 반환
        logger.info("FINISH - Result:" + svcResult + " (TimeElapsed:" + (System.currentTimeMillis() - startTime) + "ms)");
        LogUtil.changeLogLayer(oldLogLayerStr);
        return svcReturn;
    }
}