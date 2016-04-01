pluralize <- function(x)
{
  last.char <- substr(x, start=nchar(x), stop=nchar(x))
  if (last.char == "s")
    paste0(x, "es")
  else
    paste0(x, "s")
}



titlecase <- function(x) 
{
  gsub(
    x, 
    pattern="(^|[[:space:]])([[:alpha:]])", 
    replacement="\\1\\U\\2", 
    perl=TRUE
  )
}
