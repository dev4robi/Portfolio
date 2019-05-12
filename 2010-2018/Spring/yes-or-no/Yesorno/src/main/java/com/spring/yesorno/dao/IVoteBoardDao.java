package com.spring.yesorno.dao;

import java.util.ArrayList;

import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.VoteBoardDto;

public interface IVoteBoardDao {

	public int selectVoteBoardRowCount() throws DataAccessException; // 게시글 개수 조회
	public VoteBoardDto selectVoteBoard(int boardId) throws DataAccessException; // 게시글 번호로 조회
	public ArrayList<VoteBoardDto> selectVoteBoardList(int begin, int dataPerPage) throws DataAccessException; // 게시글 (begin)~(begin+dataPerPage) 조회
	public int insertVoteBoard(VoteBoardDto voteBoardDto) throws DataAccessException; // 게시글 추가
	public int updateVoteBoardContent(int boardId, String modifiedContent) throws DataAccessException; // 게시글 본문내용 수정
	public int deleteVoteBoard(int boardId) throws DataAccessException; // 게시글 번호로 삭제
}
