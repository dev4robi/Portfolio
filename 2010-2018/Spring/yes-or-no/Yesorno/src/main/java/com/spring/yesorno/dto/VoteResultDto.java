package com.spring.yesorno.dto;

/** 투표 결과를 담음 */
public class VoteResultDto {

	private int voteAgreePercent;	// 찬성 백분율
	private int voteAgreeCnt;		// 찬성 수
	private int voteDisagreeCnt;	// 반대 수
	private String voteResult;		// 투표 결과

	public int getVoteAgreePercent() {
		return voteAgreePercent;
	}

	public void setVoteAgreePercent(int voteAgreePercent) {
		this.voteAgreePercent = voteAgreePercent;
	}

	public int getVoteAgreeCnt() {
		return voteAgreeCnt;
	}

	public void setVoteAgreeCnt(int voteAgreeCnt) {
		this.voteAgreeCnt = voteAgreeCnt;
	}

	public int getVoteDisagreeCnt() {
		return voteDisagreeCnt;
	}

	public void setVoteDisagreeCnt(int voteDisagreeCnt) {
		this.voteDisagreeCnt = voteDisagreeCnt;
	}

	public String getVoteResult() {
		return voteResult;
	}

	public void setVoteResult(String voteResult) {
		this.voteResult = voteResult;
	}
}
