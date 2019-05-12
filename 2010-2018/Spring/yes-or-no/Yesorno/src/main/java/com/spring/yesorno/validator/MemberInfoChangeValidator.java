package com.spring.yesorno.validator;

import org.springframework.validation.Errors;
import org.springframework.validation.ValidationUtils;
import org.springframework.validation.Validator;

import com.spring.yesorno.command.MemberInfoChangeCmd;
import com.spring.yesorno.dto.MemberDto;

public class MemberInfoChangeValidator implements Validator {
	
	@Override
	public boolean supports(Class<?> clazz) {
		return MemberInfoChangeValidator.class.isAssignableFrom(clazz);
	}

	@Override
	public void validate(Object target, Errors errors) {
		final MemberInfoChangeCmd cmd = (MemberInfoChangeCmd)target;
		final String memberNickname = cmd.getMemberNickname();

		// memberNickname 양식 확인
		ValidationUtils.rejectIfEmptyOrWhitespace(errors, "memberNickname", "error.required");

		// memberNickname 길이 확인
		if (memberNickname.length() > 16) {
			errors.rejectValue("memberNickname", "error.lengthOver");
		}
	}
}
