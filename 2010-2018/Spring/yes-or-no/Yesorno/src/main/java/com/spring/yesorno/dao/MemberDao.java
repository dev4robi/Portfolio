package com.spring.yesorno.dao;

import org.mybatis.spring.SqlSessionTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.MemberDto;

public class MemberDao implements IMemberDao {

	@Autowired private SqlSessionTemplate sqlSession;

	// 회원 닉네임 조회
	public String selectMemberNickname(int memberId) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.selectMemberNickname(memberId);
	}
	
	// 회원 조회
	public MemberDto memberSelect(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberSelect(memberDto);
	}
	
	// 회원 가입
	public int memberInsert(MemberDto memberDto) throws DataAccessException { 			
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberInsert(memberDto);
	}
	
	// 회원 탈퇴 정보 업데이트
	public int memberDeregistrationUpdate(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberDeregistrationUpdate(memberDto);
	}
	
	// 회원정보 수정
	public int memberInfoUpdate(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberInfoUpdate(memberDto);
	}
	
	// 회원 로그인 정보 업데이트
	public int memberLoginUpdate(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberLoginUpdate(memberDto);
	}
}
