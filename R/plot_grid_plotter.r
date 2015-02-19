grid_plotter <- function(plots, which, label, show.title=TRUE, legend)
{
  add.legend <- !missing(legend)
  
  # Error checking
  if (length(which) > 4 || length(which) < 1 || any(which < 0) || any(which > 4) || length(unique(which)) != length(which))
  {
    stop("argument 'which' must contain a subset of the numbers 1, 2, 3, 4")
  } 
  
  # 3-4 plots
  else if (length(which) >= 3)
  {
    grid <- c(2, 2)
    g <- grid.layout(2, 2)
  } 
  
  # 2 plots
  else if (length(which) == 2)
  {
    grid <- c(1, 2)
    g <- grid.layout(1, 2)
  } 
  
  # 1 plot
  else
  {
    grid <- c(1, 1)
    g <- grid.layout(1, 1)
  }
  
  ### Plot
  k <- 1L
  
#  if (show.title)
#    adj <- 1L
#  else
    adj <- 0L
  
  g <- frameGrob(name="g", layout=g)
  
  for (i in 1L:grid[1L] + adj)
  {
    for (j in 1L:grid[2L]){
      if (k == length(which) && k < prod(grid))
        j <- j:(j + 1L)
        
      g <- placeGrob(g, ggplotGrob(plots[[which[k]]]), row=i, col=j)
      k <- k + 1L
      
      if (k > length(which))
        break
    }
    
    if (k > length(which))
      break
  }
  
  # render
  if (!show.title)
    label <- NULL
  
  label <- paste("\n", label, sep="")
  
  if (missing(legend))
    plots <- arrangeGrob(g, main=label)
  else
    plots <- arrangeGrob(g, main=label, legend, ncol=2, widths=c(10, 2))
  
  return( plots )
}


