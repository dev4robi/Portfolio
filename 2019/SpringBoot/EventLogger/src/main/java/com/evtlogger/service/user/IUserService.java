package com.evtlogger.service.user;

import com.evtlogger.dao.UserDao;
import com.evtlogger.etc.ServiceReturn;

public interface IUserService {
	
	public ServiceReturn signup(UserDao userDao);                          // 회원 가입
	public ServiceReturn deregist(String userJwt, String userId);          // 회원 탈퇴
	public ServiceReturn login(String userId);                             // 로그인
	public ServiceReturn selectUser(String userId);                        // 회원정보 조회
	public ServiceReturn checkUser(String userJwt, String userId);         // 회원 검사 
}
