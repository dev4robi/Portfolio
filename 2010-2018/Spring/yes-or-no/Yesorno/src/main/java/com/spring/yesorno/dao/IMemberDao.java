package com.spring.yesorno.dao;

import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.MemberDto;

public interface IMemberDao {

	public String selectMemberNickname(int memberId) throws DataAccessException;			// 회원 닉네임 조회
	public MemberDto memberSelect(MemberDto memberDto) throws DataAccessException;			// 회원  조회
	public int memberInsert(MemberDto memberDto) throws DataAccessException; 				// 회원 가입
	public int memberDeregistrationUpdate(MemberDto memberDto) throws DataAccessException;	// 회원 탈퇴 정보 업데이트
	public int memberInfoUpdate(MemberDto memberDto) throws DataAccessException;			// 회원정보 수정
	public int memberLoginUpdate(MemberDto memberDto) throws DataAccessException;			// 회원 로그인 정보 업데이트
}
