package com.spring.yesorno.dto;

import java.util.Date;

/** 투표 진행상황 DB접근 */
public class VoteProgressDto {

	private int voteProgressId;				// INT				PK,AI		투표 진행 번호
	private int voteBoardId;				// INT 				FK(ODC),NN	투표 진행 게시판 번호
	private int voterMemberId;				// INT				FK(ODR),NN	투표자 번호
	private int voteResult;					// INT				NN			투표 결과 (+1:찬성, -1반대)
	private Date voteDate;					// DATETIME			NN			투표 날자
	
	public int getVoteProgressId() {
		return voteProgressId;
	}
	
	public void setVoteProgressId(int voteProgressId) {
		this.voteProgressId = voteProgressId;
	}
	
	public int getVoteBoardId() {
		return voteBoardId;
	}
	
	public void setVoteBoardId(int voteBoardId) {
		this.voteBoardId = voteBoardId;
	}
	
	public int getVoterMemberId() {
		return voterMemberId;
	}
	
	public void setVoterMemberId(int voterMemberId) {
		this.voterMemberId = voterMemberId;
	}

	public int getVoteResult() {
		return voteResult;
	}

	public void setVoteResult(int voteResult) {
		this.voteResult = voteResult;
	}

	public Date getVoteDate() {
		return voteDate;
	}
	
	public void setVoteDate(Date voteDate) {
		this.voteDate = voteDate;
	}
}
