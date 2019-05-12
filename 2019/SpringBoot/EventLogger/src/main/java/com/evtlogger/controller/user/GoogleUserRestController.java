package com.evtlogger.controller.user;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.evtlogger.service.user.GoogleUserService;

@RestController
public class GoogleUserRestController {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(GoogleUserRestController.class);
	
	// [Class private variables]
	@Autowired @Qualifier("googleUserService")
	private GoogleUserService userSvcGoogle;
	
	// [Methods]
	@PostMapping("user/signup/google") // 구글로 회원가입
	public Map<String, Object> googleSignup(@RequestParam(value = "idToken", required = true) String idToken) {
		return userSvcGoogle.googleSignup(idToken).toMap();
	}
	
	@PostMapping("user/login/google") // 구글로 로그인
	public Map<String, Object> googleLogin(@RequestParam(value = "idToken", required = true) String idToken) {
		return userSvcGoogle.googleLogin(idToken).toMap();
	}
}
