package com.evtlogger.aop;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.Signature;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

import com.evtlogger.manager.MsgCodeManager;
import com.evtlogger.util.LogUtil;

@Aspect
@Component
public class CtrAspect {
	// [Class public constants]
    public static final String CTR_KEY_VERSION_MSG          = "_version";
    public static final String CTR_KEY_TID					= "_tId";
	public static final String CTR_KEY_RESULT				= "_result";
	public static final String CTR_KEY_RESULT_CODE			= "_resultCode";
	public static final String CTR_KEY_RESULT_MSG			= "_resultMsg";
	public static final String CTR_KEY_RESPONSE_DEFAULT		= "response";
	
	public static final String CTR_RESULT_SUCCESS		    = "SUCCESS";
	public static final String CTR_RESULT_FAIL			    = "FAIL";
	public static final String CTR_RESULT_CODE_SUCCESS	    = "00000";
	public static final String CTR_RESULT_CODE_ERROR	    = "99999";
	
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(CtrAspect.class);
	@Autowired private MsgCodeManager msgCodeManager;
	
	// [Notes]
	// @Around() : 메시지의 실행 전/후
	// @Before() : 메서드 실행 전
	// @After()  : 메서드 실행 후
	
	// [Methods]
	@Around("execution(* com.evtlogger.controller..*.*(..))") // 컨트롤러 로깅
	public Object controllerLogging(ProceedingJoinPoint pjp) throws Throwable {
        // 기존 로그 계층(layer) 저장, 새 로그계층 적용 [CTR]
		final String prevLogLayer = LogUtil.changeLogLayer(LogUtil.LAYER_CTR);
		
		// HttpServletRequest 추출
		ServletRequestAttributes servletReqAttr = (ServletRequestAttributes)RequestContextHolder.getRequestAttributes();
		HttpServletRequest request = servletReqAttr.getRequest();
		
		if (request == null) {
			Signature sign = pjp.getSignature();
			String packageName = sign.getDeclaringTypeName();
			String methodName = sign.getName();
			throw new Exception("Controller '" + packageName + "." + methodName + "()' fail to load HttpServletRequest!");	
		}
		
		// 트랜잭션 ID생성
		// ...
		long tId = 0; // 임시 tId...
		
		// 접속 IP 주소 로깅
		String cliAddr = request.getHeader("X-FORWARDED-FOR"); // Client real IP
		
		if (cliAddr == null) {
			cliAddr = request.getRemoteAddr();
		}
		
		logger.info("BEGIN - " + request.getMethod() + " '"+ request.getRequestURI() + "' (IP:" + cliAddr + ")");
		
		// 컨트롤러 수행 시작
		boolean ctrExceptionCaused	= false;	// 컨트롤러 예외 발생여부
		boolean ctrResult			= false;	// 컨트롤러 수행 결과
		Object ctrReturn			= null;		// 컨트롤러에서 반환하는 값 (ControllerReturn.class)
		
		try {
			ctrReturn = pjp.proceed(); // 컨트롤러 수행
		}
		catch (Exception e) {
			logger.error("Controller Exception!", e);
			ctrExceptionCaused = true;
			ctrResult = false;
		}
		
		// 컨트롤러 결과값 수정
		String responseCode = null;
		Map<String,Object> responseMap = null;
		
		if (!ctrExceptionCaused) {
			// 컨트롤러가 Map을 반환시 사용자 정의 헤더 추가
			if (ctrReturn instanceof Map) {
				@SuppressWarnings("unchecked")
				Map<String, Object> returnMap = (Map<String, Object>) ctrReturn;
				responseCode	= (String)returnMap.get(CTR_KEY_RESULT_CODE);
				responseMap  = returnMap;
				
				if (responseCode == null) {
					Signature sign = pjp.getSignature();
					String packageName = sign.getDeclaringTypeName();
					String methodName = sign.getName();
					logger.error("'responseCode' is null! Controller '" + packageName + "." + methodName + "()' does not use 'ServiceReturn.toMap()'?");
					responseCode = CTR_RESULT_CODE_ERROR;
					responseMap.put(CTR_KEY_RESULT_CODE, responseCode);
				}
				else if (responseCode.equals(CTR_RESULT_CODE_SUCCESS)) {
					ctrResult = true;
				}
				
				// 기본 응답헤더 추가 ( '_tId' / '_result' / '_resultMsg' / '_version' )
				responseMap.put(CTR_KEY_TID,			String.valueOf(tId));
				responseMap.put(CTR_KEY_RESULT,			(ctrResult ? CTR_RESULT_SUCCESS : CTR_RESULT_FAIL));
				responseMap.put(CTR_KEY_RESULT_MSG,		msgCodeManager.getMsg(responseCode));
				responseMap.put(CTR_KEY_VERSION_MSG,    "0.0.0");
				
				// [#]test logging
				StringBuilder responseSb = new StringBuilder();
				responseSb.append('{');
				
				if (responseMap.size() > 0) {
					for (String key : responseMap.keySet()) {
						responseSb.append('\"').append(key).append("\":\"").append(responseMap.get(key)).append("\",");
					}
				
					responseSb.setLength(responseSb.length() - 1);
				}
				
				responseSb.append('}');
				logger.info("RESPONSE - " + responseSb.toString());
				// [#]test logging
				
				ctrReturn = responseMap;
			}
			// 컨트롤러가 다른 값들을 반환하면 스프링 디폴트로 처리
			else { /* ... */ }
		}

		logger.info("E N D - " + request.getMethod() + " '"+ request.getRequestURI());

		// 로그 계층 되돌리고 결과 반환
		LogUtil.changeLogLayer(prevLogLayer);
		return ctrReturn;
	}
}
