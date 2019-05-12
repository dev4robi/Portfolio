package com.spring.yesorno.service;

import java.util.Date;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.ui.Model;
import org.springframework.validation.Errors;

import com.spring.yesorno.command.MemberInfoChangeCmd;
import com.spring.yesorno.command.MemberLoginCmd;
import com.spring.yesorno.command.MemberRegistrationCmd;
import com.spring.yesorno.dao.MemberDao;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.exception.MemberException;
import com.spring.yesorno.util.JwtMemberAuth;

public class MemberService {

	@Autowired private MemberDao memberDao;
	@Autowired private JwtMemberAuth jwtMemberAuth;
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	// 회원토큰 생성(쿠키)
	public void generateMemberToken(String memberToken, HttpServletResponse response) {
		Cookie memberCookie = new Cookie("memberToken", memberToken);
		memberCookie.setPath("/");
		memberCookie.setComment("memberToken");
		memberCookie.setMaxAge((int)JwtMemberAuth.TokenExpireMinute);
		response.addCookie(memberCookie);
	}
	
	// 회원토큰 제거(쿠키)
	public void removeMemberToken(HttpServletResponse response) {
		Cookie memberCookie = new Cookie("memberToken", null);
		memberCookie.setPath("/");
		memberCookie.setComment("memberToken");
		memberCookie.setMaxAge(0);
		response.addCookie(memberCookie);
	}
	
	// 회원 권한 확인
	public boolean checkMemberGrade(int memberId, MemberDto.EnumMemberGradeId memberGradeOver) {
		boolean svcResult = false;
		MemberDto checkMember = new MemberDto();
		checkMember.setMemberId(memberId);
		
		if ((checkMember = memberDao.memberSelect(checkMember)) != null) { // 회원정보 있음
			if (memberGradeOver.getValue() <= checkMember.getMemberGradeId()) { // 요청 권한 이상
				svcResult = true;
			} else { // 요청 권한 이하
				svcResult = false;
			}
		} else { // 회원정보 없음
			svcResult = false;
		}
		
		return svcResult;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	// 회원 가입
	public boolean memberRegistration(MemberRegistrationCmd cmd, Errors errors) {
		boolean svcResult = false;

		try {
			MemberDto registerMember = new MemberDto();
			registerMember.setMemberEmail(cmd.getMemberEmail());								// 이메일
			registerMember.setMemberNickname(cmd.getMemberNickname());							// 닉네임
			registerMember.setMemberGradeId(MemberDto.EnumMemberGradeId.MG_NORMAL.getValue());	// 등급
			registerMember.setMemberJoinDate(new Date());										// 가입일
			svcResult = (memberDao.memberInsert(registerMember) == 1 ? true : false);
		} catch (DataAccessException e) {
			if (e instanceof DuplicateKeyException) { // 이메일 혹은 닉네임 중복 
				String errorMsg = e.getMessage();
				if (errorMsg.contains("for key 'MEMBER_EMAIL'")) { // 이메일 중복
					errors.rejectValue("memberEmail", "error.alreadyUsed");
				} else { // 닉네임 중복
					errors.rejectValue("memberNickname", "error.alreadyUsed");
				}
			}

			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	// 회원 탈퇴
	public boolean memberDeregistration(String memberToken, HttpServletResponse response) throws MemberException {
		boolean svcResult = false;
		MemberDto deregistrationMember = null;

		try {
			// 회원토큰 인증
			if ((deregistrationMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
			
			// 회원정보 조회
			if ((deregistrationMember = memberDao.memberSelect(deregistrationMember)) != null) {
				deregistrationMember.setMemberGradeId(MemberDto.EnumMemberGradeId.MG_RETIRED.getValue());
				deregistrationMember.setMemberLastLoginDate(new Date());
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}

			// 회원탈퇴 업데이트
			if (memberDao.memberDeregistrationUpdate(deregistrationMember) == 1) {
				removeMemberToken(response);
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_UPDATE_FAIL);
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	// 회원 로그인
	public boolean memberLogin(MemberLoginCmd cmd, HttpServletResponse response, Errors errors) throws MemberException {
		boolean svcResult = false;

		try {
			MemberDto loginMember = new MemberDto();
			loginMember.setMemberEmail(cmd.getMemberEmail());
		
			// 회원정보 조회
			if ((loginMember = memberDao.memberSelect(loginMember)) != null ? true : false) {} 
			else {
				svcResult = false;
				errors.rejectValue("memberEmail", "error.login.memberEmail");
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}
			
			// 계정 등급 확인
			if (loginMember.getMemberGradeId() != MemberDto.EnumMemberGradeId.MG_RETIRED.getValue()) {}			
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_BANNED_MEMBER);
			}
			
			if (loginMember.getMemberGradeId() != MemberDto.EnumMemberGradeId.MG_BANNED.getValue()) {
				loginMember.setMemberLastLoginDate(new Date());
			}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_BANNED_MEMBER);
			}

			// 로그인 기록 업데이트
			if (memberDao.memberLoginUpdate(loginMember) == 1) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_UPDATE_FAIL);
			}
			
			// 맴버토큰 생성
			String memberToken = null;
			if ((memberToken = jwtMemberAuth.createMemberToken(loginMember)) != null) {
				generateMemberToken(memberToken, response);
				svcResult = true;
			} else {
				errors.rejectValue("memberEmail", "error.token.generate");
				return false;
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	// 회원 로그아웃
	public boolean memberLogout(String memberToken, HttpServletResponse response) {
		boolean svcResult = false;

		// 회원토큰 인증
		if (jwtMemberAuth.authMemberToken(memberToken)) {
			removeMemberToken(response);
			svcResult = true;
		} else {
			svcResult = false;
			throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
		}

		return svcResult;
	}
	
	// 회원정보 변경
	public boolean memberInfoChange(String memberToken, MemberInfoChangeCmd cmd, HttpServletResponse response, Errors errors) {		
		boolean svcResult = false;
		MemberDto infoChangeMember = null;

		try {
			// 회원토큰 인증
			if ((infoChangeMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {
				infoChangeMember.setMemberNickname(cmd.getMemberNickname());
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
				
			// 회원정보 변경
			if (memberDao.memberInfoUpdate(infoChangeMember) == 1) {
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_UPDATE_FAIL);
			}
			
			// 맴버토큰 생성
			memberToken = null;
			if ((memberToken = jwtMemberAuth.createMemberToken(infoChangeMember)) != null) {
				generateMemberToken(memberToken, response);
				svcResult = true;
			} else {
				errors.rejectValue("memberEmail", "error.token.generate");
				return false;
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			if (e instanceof DuplicateKeyException) { // 중복 닉네임
				errors.rejectValue("memberNickname", "error.alreadyUsed");
			}

			svcResult = false;
		}

		return svcResult;
	}

	//////////	//////////	//////////	//////////	//////////	//////////	//////////

	// 회원정보 변경 페이지
	public boolean memberInfoChangePage(String memberToken, MemberInfoChangeCmd cmd, Errors errors) {		
		boolean svcResult = false;
		MemberDto infoChangeMember = null;

		try {
			// 회원정보 인증
			if ((infoChangeMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
			
			// 회원정보 조회
			if ((infoChangeMember = memberDao.memberSelect(infoChangeMember)) != null) {
				cmd.setMemberEmail(infoChangeMember.getMemberEmail());
				cmd.setMemberNickname(infoChangeMember.getMemberNickname());
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	public boolean memberDeregistrationPage(String memberToken, Model model) {
		boolean svcResult = false;
		MemberDto deregistMember = new MemberDto();
		
		try {
			// 회원토큰 인증
			if ((deregistMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
			
			// 회원정보 조회
			if ((deregistMember = memberDao.memberSelect(deregistMember)) != null) {
				model.addAttribute("deregistrationMember", deregistMember);
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
}
