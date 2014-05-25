papi.event.init <- function(which)
{
	ret <- .Call("papi_event_counter_init",which) # ret: name to id conversion
	return(ret)
}

papi.event.start <- function(which)
{
	ret <- .Call("papi_event_counter_on",which) # ret: error value
	return(ret)
}

papi.event.stop <- function(which)
{
	ret <- .Call("papi_event_counter_off",which) # ret: counter values
	return(ret)
}

papi.event.simple <- function(expr, which)
{
	which <- papi.event.init(which)

	ret <- papi.event.start(which)
	if (!is.integer(ret))
		stop(ret)

	if (!missing("expr"))
		expr

	ret <- papi.event.stop(which)

	if (length(ret) == 1L && is.integer(ret) && ret == -1L)
		stop("There was a problem recovering the counter information.")

	return(ret)
}
