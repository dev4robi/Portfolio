package com.spring.yesorno.util;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;

public class JsonParser {

	private ObjectMapper mapper;
	
	public JsonParser() {
		mapper = new ObjectMapper();
	}

	// JSON문자열을 Map으로 반환
	public Map<String, Object> mapFromJsonString(String jsonString) throws IOException, JsonParseException, JsonMappingException {
		Map<String, Object> resultMap = new HashMap<String, Object>();

		resultMap = mapper.readValue(jsonString, new TypeReference<Map<String, Object>>(){});
		
		return resultMap; 
	}
	
	// Map을 JSON문자열로 반환
	public String jsonStringFromMap(Map<String, Object> map) throws IOException, JsonParseException, JsonMappingException {
		return mapper.writeValueAsString(map);
	}
}
