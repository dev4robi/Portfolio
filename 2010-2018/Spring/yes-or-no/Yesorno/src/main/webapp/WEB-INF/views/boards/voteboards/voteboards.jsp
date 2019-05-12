<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<%@ taglib uri="http://www.springframework.org/tags/form" prefix="form" %>
<html>
<head>
	<!-- Include libraries(jQuery, bootstrap) -->
	<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<!-- Include summernote css/js -->
	<link href="http://cdnjs.cloudflare.com/ajax/libs/summernote/0.8.9/summernote.css" rel="stylesheet">
	<script src="http://cdnjs.cloudflare.com/ajax/libs/summernote/0.8.9/summernote.js"></script>
	<!-- Title -->
	<title><spring:message code="voteboards.title"/></title>
</head>
<body>
	<!-- Strings -->
	<input type="hidden" id="boardId" value="${voteBoardRead.voteBoardId}"/>
	<input type="hidden" id="alreadyVote" value='<spring:message code="voteboards.alreadyVoted"/>'/>
	<input type="hidden" id="needLogin" value='<spring:message code="members.needLogin"/>'/>
	<input type="hidden" id="memberGradeError" value='<spring:message code="members.gradeError"/>'/>
	<!-- Body -->
	<div class="container" style="padding-top: 100px;">
		<h2 class="page.headder">${voteBoardRead.voteBoardTitle}</h2> <%-- 게시글 제목 --%>
		<table id="content_table" class="table table-bordered">
			<tr> <%-- 게시글 썸네일 --%>
				<td class="align-middle"><img src="${voteBoardRead.voteBoardImageURL}"></td>
			</tr>
			<tr> <%-- 게시글 내용 --%>
				<td>
					<div id="voteBoardContent" style="display: block">${voteBoardRead.voteBoardContent}</div> <%-- 게시글 내용 --%>
					<div id="voteBoardContentModify" style="display: none"> <%-- 게시글 수정 --%>
						<textarea class="form-control" id="summernote" name="content" placeholder="content" maxlength="140" rows="7"></textarea>
					</div>
				</td>
			</tr>
			<tr> <%-- 투표 결과 및 투표 --%>
				<c:choose>
					<c:when test="${voteBoardRead.voteAgreeCnt + voteBoardRead.voteDisagreeCnt > 0}">
						<c:set var="barWithVoterStyle" value="block"/>
						<c:set var="barNoVoterStyle" value="none"/>
					</c:when>
					<c:otherwise>
						<c:set var="barWithVoterStyle" value="none"/>
						<c:set var="barNoVoterStyle" value="block"/>
					</c:otherwise>
				</c:choose>
				<td class="align-middle">
					<div id="progressBarWithVoter" style="display: ${barWithVoterStyle}"> <%-- 투표자가 있을 때 --%>
						<div class="progress">
							<div id="agreeProgressBar" class="progress-bar progress-bar-success" role="proressbar" style="width:${voteBoardRead.voteAgreePercent}%">
								<spring:message code="boards.agree"/> (${voteBoardRead.voteAgreePercent}%)
							</div>
							<div id="disagreeProgressBar" class="progress-bar progress-bar-danger" role="progressbar" style="width:${100-voteBoardRead.voteAgreePercent}%">
								<spring:message code="boards.disagree"/> (${100-voteBoardRead.voteAgreePercent}%)
							</div>
						</div>
					</div>
					<div id="progressBarNoVoter" style="display: ${barNoVoterStyle}"> <%-- 투표자가 없을 때 --%>
						<div class="progress">
							<div class="progress-bar progress-bar-warning" role="progressbar" style="width:100%">
								<spring:message code="boards.zeroVoter"/>	
							</div>
						</div>
					</div>
					<c:set var="voteProgressesURL" value="../../voteprogresses/${voteBoardRead.voteBoardId}"/>
					<button id="voteAgreeBtn" type="button" class="btn btn-success"><spring:message code="boards.agree"/> (${voteBoardRead.voteAgreeCnt})</button>
					<button id="voteDisagreeBtn" type="button" class="btn btn-danger"><spring:message code="boards.disagree"/> (${voteBoardRead.voteDisagreeCnt})</button>
				</td>
			</tr>
			 <%-- 작성자가 읽는중이면 게시글 수정 및 삭제 허용 --%>
			<c:choose>
				<c:when test="${voteBoardRead.readerIsWriter == true}">
					<tr>
						<td>
							<div id="modifyAndDeleteBtn" style="display: block"> <%-- 글수정/삭제 --%>
								<button id="voteBoardModify" type="button" class="btn btn-success"><spring:message code="boards.modify"/></button>
								<button id="voteBoardDelete" type="button" class="btn btn-success"><spring:message code="boards.delete"/></button>
							</div>
							<div id="modifyAndCancleBtn" style="display: none"> <%-- 수정/취소 --%>
								<button id="voteBoardModifyAjax" type="button" class="btn btn-success"><spring:message code="boards.modify"/></button>
								<button id="voteBoardModifyCancle" type="button" class="btn btn-success"><spring:message code="boards.cancle"/></button>
							</div>
						</td>
					</tr>
				</c:when>
			</c:choose>
		</table>
		<%-- 덧글 --%>
		<table id="comment_table" class="table table-bordered">
			<%-- 마지막 덧글 위치 --%>
			<tr id="nextCommentRow"></tr>
			<%-- 덧글 입력란 --%>
			<tr><td>
				<div class="form-group">
  					<label for="comment"><spring:message code="comments.title"/></label>
  					<textarea class="form-control" rows="5" id="commentTextArea"></textarea>
				</div>
				<button id="writeCommentBtn" type="button" class="btn btn-success"><spring:message code="comments.write"/></button>
			</td></tr>
		</table>
	</div>
</body>
	<script>
		<%-- 컨텍스트 경로 반환 --%>
		function getContextPath() {
			var hostIndex = location.href.indexOf( location.host ) + location.host.length;
			return location.href.substring( hostIndex, location.href.indexOf('/', hostIndex + 1) );
		};

		<%-- 게이지바 UI 수정 --%>
		function refreshVoteUI(uiHide, result) {
			if (result.voteAgreeCnt + result.voteDisagreeCnt == 0) { <%-- 투표자 없음 --%>
				$("#progressBarWithVoter").css("display", "none");
				$("#progressBarNoVoter").css("display", "block");
			} else { <%-- 투표자 있음 --%>
				$("#progressBarWithVoter").css("display", "block");
				$("#progressBarNoVoter").css("display", "none");
				
				$("#voteAgreeBtn").html("<spring:message code="boards.agree"/> (" + result.voteAgreeCnt + ")");
				$("#voteDisgreeBtn").html("<spring:message code="boards.disagree"/> (" + result.voteDisagreeCnt + ")");
				
				$("#agreeProgressBar").css("width", (result.voteAgreePercent + "%"));
				$("#agreeProgressBar").html("<spring:message code="boards.agree"/> (" + result.voteAgreePercent + "%)");
				
				$("#disagreeProgressBar").css("width", ((100 - result.voteAgreePercent) + "%"));
				$("#disagreeProgressBar").html("<spring:message code="boards.disagree"/> (" + ((100 - result.voteAgreePercent) + "%)"));
			}
		}
		
		<%-- 글수정 UI 수정 --%>
		function refreshModifyUI(deleteBtnHide) {
			if (deleteBtnHide) {
				$("#modifyAndDeleteBtn").css("display", "none");
				$("#modifyAndCancleBtn").css("display", "block");
				$("#voteBoardContent").css("display", "none");
				$("#voteBoardContentModify").css("display", "block");
			} else {
				$("#modifyAndDeleteBtn").css("display", "block");
				$("#modifyAndCancleBtn").css("display", "none");
				$("#voteBoardContent").css("display", "block");
				$("#voteBoardContentModify").css("display", "none");
			}
		}
		
		<%-- 투표 결과 --%>
		function voteResult(result) {
			if(result.voteResult == 'alreadyVote'){
				alert($("#alreadyVote").val());
				return false;
			} else if (result.voteResult == 'needLogin'){
				alert($("#needLogin").val());
				return false;
			} else if (result.voteResult == 'memberGradeError') {
				alert($("#memberGradeError").val());
				return false;
			} else {
				refreshVoteUI(result);
				return true;
			}
		}
		
		<%-- 투표 새로고침 ajax --%>
		function ajaxRefreshVote() {
			$.ajax({
				url : getContextPath() + "/voteprogresses/" + $("#boardId").val(),
				type : "GET",
				data : {},
				success : function(result) { refreshVoteUI(result); }
			});
		}
		
		<%-- 투표(찬성) ajax --%>
		$("#voteAgreeBtn").bind("click",function() {
			$.ajax({
				url : getContextPath() + "/voteprogresses/" + $("#boardId").val() + "/agree",
				type: "POST",
				data : {},
				success : function(result) { voteResult(result); }
			});
		});
		
		<%-- 투표(반대) ajax --%>
		$("#voteDisagreeBtn").bind("click",function() {
			$.ajax({
				url : getContextPath() + "/voteprogresses/" + $("#boardId").val() + "/disagree",
				type: "POST",
				data : {},
				success : function(result) { voteResult(result); }
			});
		});
		
		<%-- 글 삭제 ajax --%>
		$("#voteBoardDelete").bind("click", function() {
			if (confirm("<spring:message code="boards.deleteWarn"/>")) {
				$.ajax({
					url : getContextPath() + "/boards/voteboards/" + $("#boardId").val(),
					type: "DELETE",
					data : {},
					success : function(result) {
						alert(getContextPath() + result);
						$(location).attr('href', (getContextPath() + "/" + result)); // redirect:/	
					},
					error : function(result) {
						alert(result);
					}
				});
			}
		});
		
		<%-- 글 수정 ajax --%>
		$("#voteBoardModifyAjax").bind("click", function() {
			var boardContent = $('.note-editable').html();
			var ajaxData = {"modifiedContent":boardContent};
			$.ajax({
				url : getContextPath() + "/boards/voteboards/" + $("#boardId").val(),
				type: "PUT",
				data : JSON.stringify(ajaxData),
				contentType:"application/json;charset=UTF-8",
				success : function(result) {
					alert("<spring:message code="boards.modifyDone"/>");
					if (result == "success") {
						boardWrite(); // summernote 내용 카피 
						refreshModifyUI(false);
					} else {
						refreshModifyUI(true);
					}
				},
				error : function(result) { alert("<spring:message code="boards.modifyErr"/>"); }
			});
		});
		
		<%-- 본문 수정 활성화/비활성화 --%>
		$("#voteBoardModify").bind("click", function() {
			var content = $("#voteBoardContent").html();
			$("#summernote").summernote("code", content);
			refreshModifyUI(true);
		});
		$("#voteBoardModifyCancle").bind("click", function() {
			refreshModifyUI(false);
		});
		$('#summernote').summernote({
			width: 800,
			height: 300,
			minHeight: 300,
			maxHeight: 1000,
			focus: false,
			toolbar: [
				['style', ['bold', 'italic', 'underline', 'clear']],
				['font', ['strikethrough', 'superscript', 'subscript']],
	            ['fontsize', ['fontsize']],
	            ['color', ['color']],
	            ['para', ['ul', 'ol', 'paragraph']],
	            ['height', ['height']],
	            ['insert', ['link', 'table']] ]
		});
		
		function boardWrite() {
	        var boardContent = $('#summernote').summernote('code');
			$('#voteBoardContent').html(boardContent);
		}
		
		<%-- 덧글 쓰기 ajax --%>
		$("#writeCommentBtn").bind("click", function() {
			var comment = $("#commentTextArea").val();
			var ajaxData = {"comment":comment};
			$.ajax({
				url : getContextPath() + "/boards/voteboards/" + $("#boardId").val() + "/comments",
				type : "POST",
				data : JSON.stringify(ajaxData),
				contentType:"application/json;charset=UTF-8",
				success : function(resultStr) {
					alert(resultStr);
					var result = JSON.parse(resultStr);
					if (result.result == "success") {
						alert("<spring:message code="comments.writeDone"/>");
					} else {
						alert("<spring:message code="comments.writeErr"/>");
					}
				},
				error : function(result) { alert("<spring:message code="comments.writeErr"/>"); }
			});
		});
	</script>
</html>