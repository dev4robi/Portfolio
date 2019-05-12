package com.spring.yesorno.command;

import java.util.Date;

import org.springframework.web.multipart.MultipartFile;

public class VoteBoardWriteCmd {

	private String voteBoardTitle;
	private String voteBoardContent;
	private MultipartFile voteBoardThumbnailImage;
	private int voteEndCnt;
	private Date voteEndDate;

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

	public MultipartFile getVoteBoardThumbnailImage() {
		return voteBoardThumbnailImage;
	}

	public void setVoteBoardThumbnailImage(MultipartFile voteBoardThumbnailImage) {
		this.voteBoardThumbnailImage = voteBoardThumbnailImage;
	}

	public int getVoteEndCnt() {
		return voteEndCnt;
	}

	public void setVoteEndCnt(int voteEndCnt) {
		this.voteEndCnt = voteEndCnt;
	}

	public Date getVoteEndDate() {
		return voteEndDate;
	}

	public void setVoteEndDate(Date voteEndDate) {
		this.voteEndDate = voteEndDate;
	}
}
