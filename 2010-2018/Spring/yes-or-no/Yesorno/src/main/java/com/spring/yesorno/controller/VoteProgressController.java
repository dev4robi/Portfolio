package com.spring.yesorno.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.spring.yesorno.dto.VoteResultDto;
import com.spring.yesorno.service.VoteProgressService;

@Controller
@RequestMapping("/voteprogresses")
public class VoteProgressController {

	@Autowired VoteProgressService voteProgressService;
	
	// 투표 결과 삽입
	// [C]POST: voteprogresses/{boardId}/{isAgree}
	@RequestMapping(value = "/{boardId}/{agreeOrDisagree}", method = RequestMethod.POST)
	public @ResponseBody VoteResultDto vote(@CookieValue(value = "memberToken", required = false) String memberToken, @PathVariable("boardId") int boardId, @PathVariable("agreeOrDisagree") String agreeOrDisagree) {
		VoteResultDto result = new VoteResultDto();

		voteProgressService.vote(memberToken, boardId, agreeOrDisagree, result);

		return result;
	}
	
	// 투표 결과 조회
	// [R]GET: voteprogresses/{boardId}
	@RequestMapping(value ="/{boardId}", method = RequestMethod.GET)
	public @ResponseBody VoteResultDto voteResult(@PathVariable("boardId") int boardId) {
		VoteResultDto result = new VoteResultDto();

		voteProgressService.getVoteResult(boardId, result);
		
		return result;
	}
}
