<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<html>
<head>
	<!-- Include libraries(jQuery, bootstrap) -->
	<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<!-- Title -->
	<title><spring:message code="voteboards.title"/></title>
</head>
<body>
	<div class="container">
		<h1><spring:message code="page.voteboards"/></h1>            
		<table class="table table-striped">
			<thead>
				<tr>
					<th><spring:message code="boards.title"/></th>
					<th><spring:message code="boards.image"/></th>
					<th><spring:message code="boards.voteResult"/></th>
					<th><spring:message code="boards.status"/></th>
	      		</tr>
	    	</thead>
	    	<tbody>
	    		<c:choose>
	    			<c:when test="${!empty voteBoardList}">
			    		<c:forEach items="${voteBoardList}" var="board">
			    			<tr>
								<td class="align-middle"><a href="../${board.voteBoardId}">${board.voteBoardTitle}</a></td>
								<td class="align-middle"><image src="${board.voteBoardImageURL}" width="75" height="50"></td>
								<td class="align-middle">
									<spring:message code="boards.agree"/>:${board.voteAgreeCnt} &#47; 
									<spring:message code="boards.disagree"/>:${board.voteDisagreeCnt}
								</td>
								<c:choose>
									<c:when test="${board.voteBoardState == 'BS_IMMINENT'}">
										<td class="text-warning"><spring:message code="voteboards.imminent"/></td>
									</c:when>
									<c:when test="${board.voteBoardState == 'BS_ENDED'}">
										<td class="text-danger"><spring:message code="voteboards.ended"/></td>
									</c:when>
									<c:otherwise>
										<td><spring:message code="voteboards.progressing"/></td>
									</c:otherwise>
								</c:choose>
							</tr>
			    		</c:forEach>
			    	</c:when>
			    	<c:otherwise>
			    		<tr>
			    			<td class="text-center" colspan="4"><spring:message code="boards.noContent"/></td>
			    		</tr>
			    	</c:otherwise>
	    		</c:choose>
	    	</tbody>
		</table>
		<a class="btn btn-primary" href="../write" role="button"><spring:message code="boards.write"/></a>
	</div>
</body>
</html>