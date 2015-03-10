title_case <- function(x) gsub(x, pattern="(^|[[:space:]])([[:alpha:]])", replacement="\\1\\U\\2", perl=TRUE)


#' Prints PAPI objects
#' 
#' @param x
#' A papi_output object to print.
#' @param ...
#' Extra arguments (ignored).
#' @param digits
#' Number of digits to print.
#' 
#' @rdname print
#' @export
print.papi_output <- function(x, ..., digits=5)
{
  maxlen <- max(sapply(names(x), nchar))
  names <- gsub(names(x), pattern="_", replacement=" ")
  names <- title_case(x=names)
  spacenames <- simplify2array(lapply(names, function(str) paste0(str, ":", paste0(rep(" ", maxlen-nchar(str)), collapse=""))))
  
  cat(paste(spacenames, sapply(x, round, digits=digits), sep=" ", collapse="\n"), "\n")
  invisible()
}



#' Subsets PAPI outputs.
#' 
#' @param x
#' PAPI object.
#' @param i
#' Index.
#' 
#' rdname subset
#' @export
"[.papi_output" <- function(x, i)
{
  class(x) <- NULL
  ret <- x[i]
  if (length(ret) > 0)
    class(ret) <- "papi_output"
  else
    return(numeric(0))
  
  return(ret)
}

