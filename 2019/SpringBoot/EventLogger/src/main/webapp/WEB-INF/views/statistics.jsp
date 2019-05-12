<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="ko">
<head>
  <!-- Meta -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <!-- Javascript and css library -->
  <!-- jquery 3.4.0 -->
  <script type="text/javascript" src="/lib/jquery-3.4.0/jquery-3.4.0.min.js"></script>
  <script type="text/javascript" src="/lib/jquery-3.4.0/jquery-cookie-1.4.1.js"></script>
  <!-- bootstrap 4.3.1 -->
  <link rel="stylesheet" href="/lib/bootstrap-4.3.1/css/bootstrap.min.css">
  <script type="text/javascript" src="/lib/bootstrap-4.3.1/js/bootstrap.min.js"></script>
  <!-- popper 1.14.7 -->
  <script type="text/javascript" src="/lib/popper-1.14.7/popper-1.14.7.js"></script>
  <script type="text/javascript" src="/lib/popper-1.14.7/tooltip-1.3.2.js"></script>
  <!-- fontawesome 5.8.1 -->
  <link rel="stylesheet" href="/lib/fontawesome-5.8.1/css/fontawesome-5.8.1.css">
  <!-- common -->
  <script type="text/javascript" src="/common/js/common.js"></script>
  <!-- skillbar: https://codepen.io/ugross/pen/zAitb -->
  <link rel="stylesheet" href="/common/skillbar/css/skillbar.css?time=<%=System.currentTimeMillis()%>">
  <script type="text/javascript" src="/common/skillbar/js/skillbar.js?time=<%=System.currentTimeMillis()%>"></script>
  <!-- statistic -->
  <script type="text/javascript" src="/common/js/statistics.js?time=<%=System.currentTimeMillis()%>"></script>
  <!-- Title -->
  <title>경조사 통계</title>
</head>
<body class="bg-dark" oncontextmenu="return false" ondragstart="return false" onselectstart="return false">
  <!-- Container -->
  <div class="container-fluid">
    <!-- Blank:Top -->
    <div class="row p-2 `border `border-primary"></div>
    <!-- Total Chart -->
    <div class="row align-items-center justify-content-center" id="div_total_chart_row">
    </div>
    <!-- Search Input & Button -->
    <div class="row align-items-center justify-content-center">
      <div class="col-11 align-self-center bg-light border-top p-2">
        <div class="input-group">
          <input type="text" class="form-control" placeholder="이름 또는 연락처로 검색" aria-describedby="button_search" id="input_search">
          <div class="input-group-append">
            <button class="btn btn-outline-secondary" type="button" id="button_search"><span class="fas fa-search"></span></button>
          </div>
        </div>
      </div>
    </div>
    <!-- Named Chart List -->
    <div class="row align-items-center justify-content-center" id="div_named_chart_rows">
    </div>
    <!-- Blank:Bottom -->
    <div class="row p-2 `border `border-primary"></div>
  </div>
</body>
</html>