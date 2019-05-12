package com.evtlogger.aop;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.Signature;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

import com.evtlogger.util.LogUtil;

@Aspect
@Component
public class SvcAspect {
    // [Class private constants]
    private static final Logger logger = LoggerFactory.getLogger(CtrAspect.class);

    // [Methods]
    @Around("execution(* com.evtlogger.service..*.*(..))") // 서비스 로깅
    public Object serviceLogging(ProceedingJoinPoint pjp) throws Throwable {
        // 기존 로그 계층(layer) 저장, 새 로그계층 적용 [SVC]
        final String prevLogLayer = LogUtil.changeLogLayer(LogUtil.LAYER_SVC);
        
        // 작업 시작 로깅
        Signature sign = pjp.getSignature();
        logger.info("BEGIN - " + sign.getDeclaringTypeName() + "." + sign.getName() + "()");
        
        Object svcReturn = null;
        long bgnTime = System.currentTimeMillis();
        
        // 서비스 수행 시작
        try {
            svcReturn = pjp.proceed(); // 서비스 수행
        }
        catch (Exception e) {
            logger.error("Service Exception!", e);
        }
        
        // 작업 종료 로깅
        long timeElapsed = System.currentTimeMillis() - bgnTime;
        logger.info("E N D - " + sign.getDeclaringTypeName() + "." + sign.getName() +
                        "() - TimeElapsed: " + timeElapsed + "ms)");
        
        // 로그 계층 되돌리고 결과 반환
        LogUtil.changeLogLayer(prevLogLayer);
        return svcReturn;
    }
}
