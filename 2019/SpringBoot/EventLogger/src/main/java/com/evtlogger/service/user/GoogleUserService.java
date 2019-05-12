package com.evtlogger.service.user;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.evtlogger.auth.GoogleAuthenticator;
import com.evtlogger.dao.UserDao;
import com.evtlogger.etc.ServiceReturn;

@Service
@Qualifier("googleUserService")
public class GoogleUserService extends BaseUserService {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(GoogleUserService.class);
	
	// [Class private variables]
	@Autowired private GoogleAuthenticator googleAuth;
	
	// [Methods]
	// 구글로 회원가입
	public ServiceReturn googleSignup(String idToken) {
	    // 파라미터 검사
		if (idToken == null) {
			logger.error("'idToken' is null!");
			return ServiceReturn.make("01301"); // 구글 토큰값이 필요합니다.
		}
		
		// 구글 OAuth 수행
		Map<String, Object> googleDataMap = null;
		
		if ((googleDataMap = googleAuth.authenticate(idToken)) == null) {
			logger.error("'googleDataMap' is null!");
			return ServiceReturn.make("01302"); // 구글 인증에 실패했습니다.
		}
		
		// signin데이터 획득 후 수행
		UserDao userDao = new UserDao();
		String userId = (String)googleDataMap.get("email");	// userId   <- email
		String userName = (String)googleDataMap.get("name");	// userName <- name
		
		userDao.setUserId(userId);
		userDao.setUserName(userName);

		return super.signup(userDao);
	}

	// 구글로 로그인
	public ServiceReturn googleLogin(String idToken) {
		// 파라미터 검사
	    if (idToken == null) {
			logger.error("'idToken' is null!");
			return ServiceReturn.make("01301"); // 구글 토큰값이 필요합니다.
		}
		
	    // login데이터 획득 후 수행
		Map<String, Object> googleDataMap = null;
		
		if ((googleDataMap = googleAuth.authenticate(idToken)) == null) {
			logger.error("'googleDataMap' is null!");
			return ServiceReturn.make("01302"); // 구글 인증에 실패했습니다.
		}
		
		return super.login((String)googleDataMap.get("email")); // userId <- email
	}
}
