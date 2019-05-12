package com.spring.yesorno.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.Errors;
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import com.spring.yesorno.command.MemberInfoChangeCmd;
import com.spring.yesorno.command.MemberLoginCmd;
import com.spring.yesorno.command.MemberRegistrationCmd;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.service.MemberService;
import com.spring.yesorno.util.JwtMemberAuth;

@Controller
public class MainController {

	@Autowired private MemberService memberService;
	@Autowired private JwtMemberAuth jwtMemberAuth;
	
	@RequestMapping("/main")
	public String mainPage(@CookieValue(value = "memberToken", required = false) String memberToken, @ModelAttribute("loginMember") MemberDto loginMember, Errors errors) {
		if (memberToken != null) {
			loginMember.copy(jwtMemberAuth.getMemberFromToken(memberToken));
			loginMember.setMemberToken(memberToken);
		}
		return "main";
	}
	
	// 회원가입 페이지
	@RequestMapping(value = "/members/register", method = RequestMethod.GET)
	public String registerPage(@ModelAttribute("memberRegistrationCmd") MemberRegistrationCmd cmd, Errors errors) {
		return "members/register";
	}
	
	// 로그인 페이지
	@RequestMapping(value = "/members/login", method = RequestMethod.GET)
	public String memberLoginPage(@ModelAttribute("memberLoginCmd") MemberLoginCmd cmd, Errors errors) {
		return "members/login";
	}
	
	// 회원정보 변경 페이지
	@RequestMapping(value = "/members/infochange", method = RequestMethod.GET)
	public String memberInfoChangePage(@CookieValue("memberToken") String memberToken, @ModelAttribute("memberInfoChangeCmd") MemberInfoChangeCmd cmd, Errors errors) {
		final String okURL  = "members/infochange";
		final String errURL = "members/login";
		String resultURL = errURL;

		try {
			if (memberService.memberInfoChangePage(memberToken, cmd, errors)) {
				resultURL = okURL; // Okay
			} else {
				resultURL = errURL; // Error
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return resultURL;
	}
	
	// 회원 탈퇴 페이지
	@RequestMapping(value = "/members/deregister", method = RequestMethod.GET)
	public String memberDeregistrationPage(@CookieValue("memberToken") String memberToken, Model model) {
		final String okURL  = "members/deregister";
		final String errURL = "members/login";
		String resultURL = errURL;

		try {
			if (memberService.memberDeregistrationPage(memberToken, model)) {
				resultURL = okURL; // Okay
			} else {
				resultURL = errURL; // Error
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return resultURL;
	}
}
