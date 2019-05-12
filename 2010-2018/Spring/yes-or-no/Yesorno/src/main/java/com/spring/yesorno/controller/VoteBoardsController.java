package com.spring.yesorno.controller;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.Errors;
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.spring.yesorno.command.VoteBoardWriteCmd;
import com.spring.yesorno.service.VoteBoardService;
import com.spring.yesorno.util.JsonParser;
import com.spring.yesorno.validator.VoteBoardWriteValidator;

@Controller
@RequestMapping("/boards/voteboards")
public class VoteBoardsController {

	@Autowired private JsonParser jsonParser;
	@Autowired private VoteBoardService voteBoardService; 
	@Autowired private VoteBoardWriteValidator voteBoardWriteValidator;
	
	private Log log = LogFactory.getLog(VoteBoardsController.class);
	
	// 투표 게시글 리스트
	@RequestMapping(value = "/list/{page}", method = RequestMethod.GET)
	public String voteBoardListByPage(HttpServletRequest request, @PathVariable("page") int page, Model model) {
		final String okURL = "boards/voteboards/list";
		final String errURL = okURL;
		String resultURL = errURL;

		if (voteBoardService.voteBoardListing(request, page, model)) {
			resultURL = okURL;
		} else {
			resultURL = errURL;
		}
		
		return resultURL;
	}
	
	// 투표 게시글 쓰기
	@RequestMapping(method = RequestMethod.POST)
	public String voteBoardWrite(HttpServletRequest request, @CookieValue(value = "memberToken", required = false) String memberToken, @ModelAttribute("voteBoardWriteCmd") VoteBoardWriteCmd cmd, Errors errors/*, @RequestParam("thumbnailImage") MultipartFile img*/) {
		final String okURL = "redirect:/boards/voteboards/list/1";
		final String errURL = "members/login";
		String resultURL = errURL;

		voteBoardWriteValidator.validate(cmd, errors);
		if (errors.hasErrors()) {
			resultURL = errURL;
		} else {
			try {
				if (voteBoardService.voteBoardWrite(memberToken, request, cmd, errors) && !errors.hasErrors()) {
					resultURL = okURL; // Okay
				} else {
					resultURL = errURL; // Error
				}
			} catch (Exception e) {
				// 임시...
				e.printStackTrace();
			}
		}
		
		return resultURL;
	}
	
	// 투표 게시글 보기
	@RequestMapping(value = "/{boardId}", method = RequestMethod.GET)
	public String voteBoardRead(HttpServletRequest request, @CookieValue(value = "memberToken", required = false) String memberToken, @PathVariable("boardId") int boardId, Model model) {
		final String okURL = "boards/voteboards/voteboards";
		final String errURL = "redirect:/boards/voteboards/list/1";
		String resultURL = errURL;

		if (voteBoardService.voteBoardReading(memberToken, request, boardId, model)) {
			resultURL = okURL;
		} else {
			resultURL = errURL;
		}

		return resultURL;
	}
	
	// 투표 게시글 수정
	// [U]PUT: /boards/voteboards/{boardId}
	@RequestMapping(value = "/{boardId}", method = RequestMethod.PUT)
	public @ResponseBody String voteBoardUpdate(@CookieValue(value = "memberToken", required = false) String memberToken, @PathVariable("boardId") int boardId, @RequestBody String jsonData) {
		final String okValue = "success";
		final String errValue = "error";
		String resultValue = errValue;

		try {
			String modifiedContent = (String)jsonParser.mapFromJsonString(jsonData).get("modifiedContent");
			if (voteBoardService.voteBoardModifyContent(memberToken, boardId, modifiedContent)) {
				resultValue = okValue;
			} else {
				resultValue = errValue;
			}
		} catch (Exception e) {
			log.debug(e.getMessage());
		}

		return resultValue;
	}
	
	// 투표 게시글 삭제
	// [D]DELETE: /boards/voteboards/{boardId}
	@RequestMapping(value = "/{boardId}", method = RequestMethod.DELETE)
	public @ResponseBody String voteBoardDelete(@CookieValue(value = "memberToken", required = false) String memberToken, @PathVariable("boardId") int boardId) {
		final String okURL = "boards/voteboards/list/1";
		final String errURL = "boards/voteboards/" + boardId;
		String resultURL = errURL;
		
		if (voteBoardService.voteBoardDelete(memberToken, boardId)) {
			resultURL = okURL;
		} else {
			resultURL = errURL;
		}

		return resultURL;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////	
	
	// 투표 게시글 쓰기 페이지
	@RequestMapping(value = "/write", method = RequestMethod.GET)
	public String voteBoardWritePage(@CookieValue(value = "memberToken", required = false) String memberToken, @ModelAttribute("voteBoardWriteCmd") VoteBoardWriteCmd cmd, Errors errors) {
		final String okURL = "boards/voteboards/write";
		final String errURL = "redirect:/members/login";
		String resultURL = okURL;
		
		if (memberToken == null || memberToken.isEmpty()) {
			resultURL = errURL;
		}
		
		return resultURL;
	}
}
