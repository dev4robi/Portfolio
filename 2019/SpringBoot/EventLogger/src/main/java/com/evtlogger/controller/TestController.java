package com.evtlogger.controller;

import java.util.Base64;
import java.util.HashMap;
import java.util.Map;

import javax.crypto.spec.SecretKeySpec;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.ModelAndView;

import com.evtlogger.etc.ServiceReturn;
import com.evtlogger.manager.MsgCodeManager;
import com.evtlogger.manager.UserJwtManager;
import com.evtlogger.service.TestService;
import com.evtlogger.util.CipherUtil;

@RestController
public class TestController {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(TestController.class);
	
	// [Class private variables]
	@Autowired private TestService testSvc;
	@Autowired private UserJwtManager userJwtManager;
	@Autowired private MsgCodeManager msgCodeManager;

	// [Methods]
	@GetMapping("/test/1")
	public String test1() {
	    return Base64.getEncoder().encodeToString((CipherUtil.encrypt(CipherUtil.AES_CBC_PKCS5, 
	            "HelloWorld!".getBytes(), new SecretKeySpec("0123456789ABCDEF".getBytes(), "AES"))));
	}
	
   @GetMapping("/test/2/{cipher}")
   public String test2(@PathVariable("cipher") String cipherStr) {
       return new String(CipherUtil.decrypt(CipherUtil.AES_CBC_PKCS5, 
               Base64.getDecoder().decode(cipherStr), new SecretKeySpec("0123456789ABCDEF".getBytes(), "AES")));
    }
   
   @GetMapping("/test/3")
   public String test3() {
       return msgCodeManager.getMsg("1001");
   }
	
	@GetMapping("/test/jwt/id={id}")
	public Map<String, Object> testJwtEncode(@PathVariable("id") String userId) throws Exception {
		String userJwt = userJwtManager.encodeUserJwt(userId);
		Map<String, Object> returnMap = new HashMap<String, Object>();
		returnMap.put("userJwt", userJwt);
		return ServiceReturn.make(returnMap).toMap();
	}
	
	@GetMapping("/test/jwt/token={token}")
	public Map<String, Object> testJwtDecode(@PathVariable("token") String jwtToken) throws Exception {
		Map<String, Object> returnMap = new HashMap<String, Object>();
		returnMap.put("decodedJwt", userJwtManager.decodeUserJwt(jwtToken));
		return ServiceReturn.make(returnMap).toMap(); 
	}
	
	@GetMapping("/test")
	public ModelAndView testEventInsert() {
		return new ModelAndView("test");
	}
}
