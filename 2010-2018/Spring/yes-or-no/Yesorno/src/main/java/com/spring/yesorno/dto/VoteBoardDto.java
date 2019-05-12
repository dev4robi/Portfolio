package com.spring.yesorno.dto;

import java.util.Date;

/** 투표 게시글 DB 접근 */
public class VoteBoardDto {

	private int voteBoardId;				// INT				PK,AI		투표게시글 번호
	private int writerMemberId;				// INT 				FK,NN		작성자 번호
	private Date voteBoardWrittenDate;		// DATETIME			NN			투표게시글 작성일
	private int voteBoardViewCnt;			// INT				NN			투표게시글 조회수
	private String voteBoardTitle;			// VARCHAR(64)		NN			투표게시글 제목
	private String voteBoardContent;		// VARCHAR(256)		NN			투표게시글 본문
	private String voteBoardImageURL;		// VARCHAR(128)					투표게시글 첨부이미지 URL
	private Date voteEndDate;				// DATETIME			NN			튜표 종료 날자
	private int voteEndCnt;					// INT							최대 투표 인원

	public void copy(VoteBoardDto voteBoardDto) {
		voteBoardId = voteBoardDto.voteBoardId;
		writerMemberId = voteBoardDto.writerMemberId;
		voteBoardWrittenDate = voteBoardDto.voteBoardWrittenDate;
		voteBoardViewCnt = voteBoardDto.voteBoardViewCnt;
		voteBoardTitle = voteBoardDto.voteBoardTitle;
		voteBoardContent = voteBoardDto.voteBoardContent;
		voteBoardImageURL = voteBoardDto.voteBoardImageURL;
		voteEndDate = voteBoardDto.voteEndDate;
		voteEndCnt = voteBoardDto.voteEndCnt;
	}

	public int getVoteBoardId() {
		return voteBoardId;
	}
	
	public void setVoteBoardId(int voteBoardId) {
		this.voteBoardId = voteBoardId;
	}
	
	public int getWriterMemberId() {
		return writerMemberId;
	}
	
	public void setWriterMemberId(int writerMemberId) {
		this.writerMemberId = writerMemberId;
	}
	
	public Date getVoteBoardWrittenDate() {
		return voteBoardWrittenDate;
	}
	
	public void setVoteBoardWrittenDate(Date voteBoardWrittenDate) {
		this.voteBoardWrittenDate = voteBoardWrittenDate;
	}
	
	public int getVoteBoardViewCnt() {
		return voteBoardViewCnt;
	}
	
	public void setVoteBoardViewCnt(int voteBoardViewCnt) {
		this.voteBoardViewCnt = voteBoardViewCnt;
	}
	
	public String getVoteBoardTitle() {
		return voteBoardTitle;
	}
	
	public void setVoteBoardTitle(String voteBoardTitle) {
		this.voteBoardTitle = voteBoardTitle;
	}
	
	public String getVoteBoardContent() {
		return voteBoardContent;
	}
	
	public void setVoteBoardContent(String voteBoardContent) {
		this.voteBoardContent = voteBoardContent;
	}
	
	public String getVoteBoardImageURL() {
		return voteBoardImageURL;
	}
	
	public void setVoteBoardImageURL(String voteBoardImageURL) {
		this.voteBoardImageURL = voteBoardImageURL;
	}
	
	public Date getVoteEndDate() {
		return voteEndDate;
	}
	
	public void setVoteEndDate(Date voteEndDate) {
		this.voteEndDate = voteEndDate;
	}
	
	public int getVoteEndCnt() {
		return voteEndCnt;
	}
	
	public void setVoteEndCnt(int voteEndCnt) {
		this.voteEndCnt = voteEndCnt;
	}
}
