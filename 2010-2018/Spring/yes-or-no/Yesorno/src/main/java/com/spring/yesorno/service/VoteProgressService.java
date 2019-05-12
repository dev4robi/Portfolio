package com.spring.yesorno.service;

import java.util.ArrayList;
import java.util.Date;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dao.VoteProgressDao;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.dto.VoteProgressDto;
import com.spring.yesorno.dto.VoteResultDto;
import com.spring.yesorno.util.JwtMemberAuth;

public class VoteProgressService {

	@Autowired private MemberService memberService;
	@Autowired private VoteProgressDao voteProgressDao;
	
	@Autowired private JwtMemberAuth jwtMemberAuth;
	
	private Log log = LogFactory.getLog(VoteProgressService.class);
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////	
	
	private int calcVoteAgreePercent(int voteAgreeCnt, int voteDisagreeCnt) {
		return (int)(voteAgreeCnt * 100.0f / (voteAgreeCnt + voteDisagreeCnt));
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	public boolean vote(String memberToken, int boardId, String agreeOrDisagree, VoteResultDto result) {
		boolean svcResult = false;
		boolean isAgree = agreeOrDisagree.equals("agree");
		
		try {
			// 회원 토큰 검사
			MemberDto voteMember = null;
			if ((voteMember = jwtMemberAuth.getMemberFromToken(memberToken)) == null) {
				result.setVoteResult("needLogin");
				return false;
			}
			
			// 회원 등급 검사
			int voteMemberId = voteMember.getMemberId();
			if (!memberService.checkMemberGrade(voteMemberId, MemberDto.EnumMemberGradeId.MG_NORMAL)) { // 회원 등급 미달
				result.setVoteResult("memberGradeError");
				return false;
			}
			
			// 중복투표 검사 및 투표 찬반개수 확인
			ArrayList<Integer> voteBoardIdList = new ArrayList<Integer>();
			voteBoardIdList.add(boardId);
			ArrayList<VoteProgressDto> voteProgressList = voteProgressDao.selectVoteProgressList(voteBoardIdList);			

			boolean voteDuplicated = false;
			int voteAgreeCnt = 0, voteDisagreeCnt = 0;
			for (VoteProgressDto voteProgressDto : voteProgressList) {
				if (voteMemberId == voteProgressDto.getVoterMemberId()) {
					voteDuplicated = true;
				}
				
				int voteResult = voteProgressDto.getVoteResult();
				if (voteResult > 0) {
					++voteAgreeCnt;
				} else if (voteResult < 0) {
					++voteDisagreeCnt;
				}
			}

			if (voteDuplicated) { // 중복투표 거부
				result.setVoteResult("alreadyVote");
				return false;
			}
			
			// 투표결과 삽입
			VoteProgressDto voteProgressDto = new VoteProgressDto();
			voteProgressDto.setVoteProgressId(0);
			voteProgressDto.setVoteBoardId(boardId);
			voteProgressDto.setVoteResult((isAgree ? 1 : -1));
			voteProgressDto.setVoterMemberId(voteMember.getMemberId());
			voteProgressDto.setVoteDate(new Date());
			
			if (voteProgressDao.insertVoteProgress(voteProgressDto) == 1) {
				result.setVoteAgreeCnt(isAgree ? ++voteAgreeCnt : voteAgreeCnt);
				result.setVoteDisagreeCnt(isAgree ? voteDisagreeCnt : ++voteDisagreeCnt);
				result.setVoteAgreePercent(calcVoteAgreePercent(voteAgreeCnt, voteDisagreeCnt));
				svcResult = true;
			}
			else {
				svcResult = false;
			}

		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		} 

		return svcResult;
	}
	
	// 해당 게시글의 투표 결과 반환
	public boolean getVoteResult(int boardId, VoteResultDto result) {
		boolean svcResult = false;
		ArrayList<Integer> voteBoardIdList = new ArrayList<Integer>();
		voteBoardIdList.add(boardId);
		
		try {
			ArrayList<VoteProgressDto> voteProgressList = voteProgressDao.selectVoteProgressList(voteBoardIdList);
			
			// 투표 개수 연산
			int voteAgreeCnt = 0, voteDisagreeCnt = 0;		
			for (VoteProgressDto voteProgressDto : voteProgressList) {
				int voteResult = voteProgressDto.getVoteResult();
				
				if (voteResult > 0) {
					++voteAgreeCnt;
				} else if (voteResult < 0) {
					++voteDisagreeCnt;
				}
			}
			
			// 결과 담고 반환
			result.setVoteAgreeCnt(voteAgreeCnt);
			result.setVoteDisagreeCnt(voteDisagreeCnt);
			result.setVoteAgreePercent(calcVoteAgreePercent(voteAgreeCnt, voteDisagreeCnt));
			svcResult = true;
			
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}

		return svcResult;
	}

	//////////	//////////	//////////	//////////	//////////	//////////	//////////
}
