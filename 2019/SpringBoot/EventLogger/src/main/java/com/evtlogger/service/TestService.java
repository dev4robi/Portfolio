package com.evtlogger.service;

import java.util.HashMap;
import java.util.Map;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.evtlogger.etc.ServiceReturn;

@Service
@Qualifier("testService")
public class TestService {

	public ServiceReturn testService(String str) throws Exception {
		Map<String, Object> rtMap = new HashMap<String, Object>();
		rtMap.put("key", str);
		return ServiceReturn.make(rtMap);
	}
}
