package com.spring.yesorno.exception;

@SuppressWarnings("serial")
public class MemberException extends RuntimeException {
	
	public enum EnumMemberExceptionType {
		ET_UNKNOWN,
		ET_TOKEN_VALIDATION_FAIL,					/** 해당 회원토큰 검증에 실패했을 때 */
		ET_DB_DATA_NOT_FOUND,						/** DB에서 해당 회원 정보를 찾지 못했을 때 */
		ET_DB_UPDATE_FAIL,							/** DB에 회원정보 업데이트 실패했을 때 */
		ET_WRONG_AUTHORITY_LEVEL,					/** 회원 등급이 해당 기능을 수행하기에 부적합할 때 */
		ET_RETIRED_MEMBER,							/** 탈퇴한 회원일 때 */
		ET_BANNED_MEMBER,							/** 사용 정지된 회원일 때 */
	}

	private EnumMemberExceptionType exceptionType;
	
	public MemberException(EnumMemberExceptionType exceptionType) {
		this.exceptionType = exceptionType;
	}

	public EnumMemberExceptionType getExceptionType() {
		return exceptionType;
	}

	public void setExceptionType(EnumMemberExceptionType exceptionType) {
		this.exceptionType = exceptionType;
	}
}
