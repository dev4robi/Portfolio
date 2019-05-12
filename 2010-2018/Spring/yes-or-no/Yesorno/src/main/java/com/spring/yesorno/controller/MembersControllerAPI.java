package com.spring.yesorno.controller;

import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import com.spring.yesorno.command.MemberInfoChangeCmd;
import com.spring.yesorno.command.MemberRegistrationCmd;
import com.spring.yesorno.exception.MemberException;
import com.spring.yesorno.service.MemberService;

@Controller
@RequestMapping("/members")
public class MembersControllerAPI {

	@Autowired MemberService memberService;
 
	@Autowired Validator memberRegistrationValidator;
	@Autowired Validator memberInfoChangeValidator;
	
	// 회원 가입
	@RequestMapping(method = RequestMethod.POST) // POST:members
	public String registerMember(@ModelAttribute("memberRegistrationCmd") MemberRegistrationCmd cmd, Errors errors) {
		final String okURL = "redirect:/main";
		final String errURL = "members/register";
		String resultURL = errURL;

		memberRegistrationValidator.validate(cmd, errors);
		if (errors.hasErrors()) {
			resultURL = errURL; // Error
		} else {
			if (memberService.memberRegistration(cmd, errors) && !errors.hasErrors()) {
				resultURL = okURL; // Okay
			} else {
				resultURL = errURL; // Error
			}
		}

		return resultURL;
	}

	// 회원정보 수정
	@RequestMapping(method = RequestMethod.PUT) // (token + PUT):members/(memberId)
	public String updateMemberInfo(@CookieValue("memberToken") String memberToken, HttpServletResponse response, @ModelAttribute("memberInfoChangeCmd") MemberInfoChangeCmd cmd, Errors errors) {
		final String okURL = "redirect:/main";
		final String errURL = "members/infochange";
		String resultURL = errURL;

		memberInfoChangeValidator.validate(cmd, errors);
		if (errors.hasErrors()) { 
			resultURL = errURL; // Error
		} else {
			try {
				if (memberService.memberInfoChange(memberToken, cmd, response, errors) && !errors.hasErrors()) {
					resultURL = okURL; // Okay
				} else {
					resultURL = errURL; // Error
				}
			} catch (MemberException e) {
				e.printStackTrace();
			}
		}

		return resultURL;
	}
	
	// 회원 탈퇴
	@RequestMapping(method = RequestMethod.DELETE) // (token + DELETE):members/(memberId)
	public String deregisterMember(@CookieValue("memberToken") String memberToken, HttpServletResponse response) {
		final String okURL = "redirect:/main";
		final String errURL = "members/deregister";
		String resultURL = errURL;

		try {
			if (memberService.memberDeregistration(memberToken, response)) {
				resultURL = okURL; // Okay
			} else {
				resultURL = errURL; // Error
			}
		} catch (MemberException e) {
			e.printStackTrace();
		}

		return resultURL;
	}
}
