package com.evtlogger.etc;

import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.evtlogger.aop.CtrAspect;
import com.evtlogger.manager.MsgCodeManager;

public class ServiceReturn {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(ServiceReturn.class);
	
	// [Class private variables]
	private String code;
	private Map<String, Object> response;
	
	// [Methods]
	// 생성자
	public ServiceReturn(String code, Map<String, Object> response) {
		setCode(code);
		setResponse(response);
	}
	
	// 컨트롤러 반환에 사용할 수 있는 맵으로 변환
	public Map<String, Object> toMap() {
		if (response == null) {
			response = new HashMap<String, Object>();
		}
		
		// 기본 응답헤더 추가( '_resultCode' / '_resultMsg' )
		response.put(CtrAspect.CTR_KEY_RESULT_CODE,	code);
		return response;
	}
	
	// 서비스 결과코드가 성공(0000)인지 검사
	public boolean checkResult() {
		if (code == null || !code.equals(MsgCodeManager.SUCCESS_CODE)) {
			return false;
		}
		
		return true;
	}
	
	// reponse로부터 key값을 가져와서 반환
	public Object get(String key) {
		if (response == null) {
			return null;
		}
		
		return response.get(key);
	}
	
	// 오류 응답코드, response를 응답 데이터에 추가
	public static ServiceReturn make(String code, Map<String, Object> response) {
		return new ServiceReturn(code, response);
	}
	
	// 오류 응답코드, 응답 데이터 없음
	public static ServiceReturn make(String code) {
		return new ServiceReturn(code, null);
	}
	
	// 응답코드 정상(0000), response를 응답 데이터에 추가
	public static ServiceReturn make(Map<String, Object> response) {
		return make(MsgCodeManager.SUCCESS_CODE, response);
	}
	
	// 응답코드 정상(0000), 배열(홀수열:key,짝수열:value)을 응답 데이터에 추가
	public static ServiceReturn make(Object... keyValAry) {
		if (keyValAry == null) {
			logger.error("'keyValAry' is null!");
			return ServiceReturn.make(MsgCodeManager.SYSTEM_FAIL_CODE);
		}
		
		if (keyValAry.length % 2 != 0) {
			logger.error("'keyValAry' is NOT multiple of 2!");
			return ServiceReturn.make(MsgCodeManager.SYSTEM_FAIL_CODE);
		}
		
		Map<String, Object> responseMap = new HashMap<String, Object>();
		String key = null;
		Object val = null;
		
		for (int i = 0; i < keyValAry.length; i += 2) {
			if (!(keyValAry[i] instanceof String)) {
				logger.warn("'keyValAry[" + i + "]' is NOT instance of String. The value '" + keyValAry[i + 1] + "' will lose!");
				continue;
			}

			key = (String)keyValAry[i];		// odd column
			val = (Object)keyValAry[i + 1];	// even column
			
			if (key == null || key.length() == 0) {
				logger.warn("'keyValAry[" + i + "]' is null or zero length. The value '" + val + "' will lose!");
				continue;
			}
			
			responseMap.put(key, val);
		}
		
		return make(MsgCodeManager.SUCCESS_CODE, responseMap);
	}
	
	// 응답코드 정상(0000), 응답 데이터 없음
	public static ServiceReturn make() {
		return make(MsgCodeManager.SUCCESS_CODE, null);
	}
	
	@Override // 문자열로 반환
	public String toString() {
	    return "(code:" + code + ")";
	}
	
	// [Getter/Setters]
	public String getCode() {
		return code;
	}
	public void setCode(String code) {
		this.code = code;
	}
	public Map<String, Object> getResponse() {
		return response;
	}
	public void setResponse(Map<String, Object> response) {
		this.response = response;
	}
}
