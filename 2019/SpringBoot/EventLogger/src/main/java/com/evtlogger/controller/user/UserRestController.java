package com.evtlogger.controller.user;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.evtlogger.service.user.BaseUserService;

@RestController
public class UserRestController {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(UserRestController.class);
	
	// [Class private variables]
	@Autowired @Qualifier("baseUserService")
	private BaseUserService userSvcBase;
	
	// [Methods]	
	@PostMapping("/user/deregist") // 회원탈퇴
	public Map<String, Object> deregist(@RequestParam(value="userJwt", required=true) String userJwt) {
		return userSvcBase.deregist(userJwt, null).toMap();
	}
}
