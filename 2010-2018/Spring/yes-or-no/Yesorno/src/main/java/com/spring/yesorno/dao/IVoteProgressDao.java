package com.spring.yesorno.dao;

import java.util.ArrayList;

import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.VoteProgressDto;

public interface IVoteProgressDao {

	public ArrayList<VoteProgressDto> selectVoteProgressList(ArrayList<Integer> voteBoardIdList) throws DataAccessException; // 매개변수 게시글ID의 투표결과들 반환
	public int insertVoteProgress(VoteProgressDto voteProgressDto) throws DataAccessException; // 투표결과 삽입
}
