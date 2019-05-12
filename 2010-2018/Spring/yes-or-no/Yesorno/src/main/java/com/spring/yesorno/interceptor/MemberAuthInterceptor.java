package com.spring.yesorno.interceptor;

import java.util.Date;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.exception.MemberException;
import com.spring.yesorno.util.JwtMemberAuth;

public class MemberAuthInterceptor extends HandlerInterceptorAdapter {

	@Autowired JwtMemberAuth jwtMemberAuth;
	
	Log log = LogFactory.getLog(MemberAuthInterceptor.class);
	
	@Override // 컨트롤러(핸들러) 실행 전
	public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
		Cookie memberTokenCookie = null;

		// 회원토큰 쿠기 검색
		for (Cookie cookie : request.getCookies()) {
			if (cookie.getName().equals("memberToken")) {
				memberTokenCookie = cookie;
				break;
			}
		}

		// 회원토큰 쿠키 검색 실패
		if (memberTokenCookie == null) {
			log.debug("> Can't find MemberTokenCookie(" + new Date() + ")");
			response.sendRedirect("members/login");
			throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
		}
		
		// 회원토큰 검증
		String memberToken = memberTokenCookie.getValue();
		MemberDto memberDto = null;
		log.debug("MemberToken: " + memberToken);
		if ((memberDto = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {
			log.debug("> Token Validation Success.");
			log.debug("> Id: " + memberDto.getMemberId());
			log.debug("> Email: " + memberDto.getMemberEmail());
			log.debug("> Nickname: " + memberDto.getMemberNickname());
			return true;
		} else {
			response.sendRedirect("members/login");
			throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
		}
	}
}
