package com.spring.yesorno.dto;

/** 투표 게시글 목록 */
public class VoteBoardListDto {

	public enum EnumVoteBoardState {
		BS_UNKOWN,
		BS_PROGRESSING,			// 진행중
		BS_IMMINENT,			// 임박
		BS_ENDED,				// 마감
	}
	
	protected int voteBoardId;						// 투표게시글 번호
	protected String voteBoardTitle;				// 투표게시글 제목
	protected String voteBoardImageURL;				// 투표게시글 이미지 경로
	protected EnumVoteBoardState voteBoardState;	// 투표게시글 상태
	protected int voteAgreeCnt;						// 투표게시글 찬성 개수
	protected int voteDisagreeCnt;					// 투표게시글 반대 개수

	public int getVoteBoardId() {
		return voteBoardId;
	}

	public void setVoteBoardId(int voteBoardId) {
		this.voteBoardId = voteBoardId;
	}

	public String getVoteBoardTitle() {
		return voteBoardTitle;
	}
	
	public void setVoteBoardTitle(String voteBoardTitle) {
		this.voteBoardTitle = voteBoardTitle;
	}
	
	public String getVoteBoardImageURL() {
		return voteBoardImageURL;
	}
	
	public void setVoteBoardImageURL(String voteBoardImageURL) {
		this.voteBoardImageURL = voteBoardImageURL;
	}
	
	public EnumVoteBoardState getVoteBoardState() {
		return voteBoardState;
	}
	
	public void setVoteBoardState(EnumVoteBoardState voteBoardState) {
		this.voteBoardState = voteBoardState;
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
}
