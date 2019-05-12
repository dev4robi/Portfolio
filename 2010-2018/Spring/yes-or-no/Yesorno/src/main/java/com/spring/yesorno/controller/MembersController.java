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

import com.spring.yesorno.command.MemberLoginCmd;
import com.spring.yesorno.service.MemberService;

@Controller
@RequestMapping("/members")
public class MembersController {

	@Autowired private MemberService memberService;
	
	@Autowired private Validator memberLoginValidator;
	
	// 로그인
	@RequestMapping(value = "/login", method = RequestMethod.POST)
	public String memberLogin(@ModelAttribute("memberLoginCmd") MemberLoginCmd cmd, HttpServletResponse response, Errors errors) {
		final String okURL  = "redirect:/main";
		final String errURL = "members/login";
		String resultURL = errURL;

		memberLoginValidator.validate(cmd, errors);
		if (errors.hasErrors()) {
			resultURL = "members/login"; // Error
		} else {
			try {
				if (memberService.memberLogin(cmd, response, errors) && !errors.hasErrors()) {
					resultURL = okURL; // Okay
				} else {
					resultURL = errURL; // Error
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		return resultURL;
	}

	// 로그아웃
	@RequestMapping(value ="/logout", method = RequestMethod.GET)
	public String memberLogout(@CookieValue("memberToken") String memberToken, HttpServletResponse response) {
		final String okURL  = "redirect:/main";
		final String errURL = okURL;
		String resultURL = errURL;

		try {
			if (memberService.memberLogout(memberToken, response)) {
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
