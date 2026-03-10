async function startSimulation(){

const traceStr=document.getElementById("trace").value.trim()
const capacity=parseInt(document.getElementById("capacity").value)
const policy=document.getElementById("policy").value

if(traceStr===""){
alert("Enter an access sequence")
return
}

const trace=traceStr.split(" ").map(Number)

await runVisualization(trace,capacity)
await runBackend(policy,capacity,traceStr)

}



async function runBackend(policy,capacity,trace){

try{

const response=await fetch("/simulate",{
method:"POST",
headers:{
"Content-Type":"application/json"
},
body:JSON.stringify({
capacity:capacity,
policy:policy,
trace:trace
})
})

const result=await response.text()

parseResults(result)

}catch(err){

console.error(err)
document.getElementById("resultsCard").innerText="Server error"

}

}



function parseResults(output){
// always show raw backend output
document.getElementById("resultsCard").style.display="block"

const resultsText=document.createElement("div")
resultsText.style.marginTop="10px"
resultsText.style.color="#00ffcc"
resultsText.innerText=output

// remove previous raw output
const old=document.getElementById("rawOutput")
if(old) old.remove()

resultsText.id="rawOutput"
document.getElementById("resultsCard").appendChild(resultsText)


// attempt structured parsing
let hits=0
let misses=0
let ratio=0

const hitMatch=output.match(/Hits:\s*(\d+)/i)
const missMatch=output.match(/Misses:\s*(\d+)/i)
const ratioMatch=output.match(/Hit Ratio:\s*([0-9.]+)/i)

if(hitMatch){
hits=hitMatch[1]
document.getElementById("hits").innerText=hits
}

if(missMatch){
misses=missMatch[1]
document.getElementById("misses").innerText=misses
}

if(ratioMatch){
ratio=ratioMatch[1]
document.getElementById("ratio").innerText=ratio
}


}



function renderCache(cache,type){

const container=document.getElementById("cacheContainer")
container.innerHTML=""

cache.forEach(val=>{

const block=document.createElement("div")
block.classList.add("cacheBlock")

if(type==="HIT") block.classList.add("cacheHit")
if(type==="MISS") block.classList.add("cacheMiss")

block.innerText=val

container.appendChild(block)

})

}



function buildTimeline(trace,results){

const timeline=document.getElementById("timeline")
timeline.innerHTML=""

for(let i=0;i<trace.length;i++){

const block=document.createElement("div")
block.classList.add("timelineBlock")

if(results[i]==="HIT"){
block.classList.add("timelineHit")
}else{
block.classList.add("timelineMiss")
}

block.innerText=trace[i]+" "+results[i]

timeline.appendChild(block)

}

}



async function runVisualization(trace,capacity){

let cache=[]
let results=[]

for(let i=0;i<trace.length;i++){

let block=trace[i]

let hit=cache.includes(block)

if(hit){

results.push("HIT")

}else{

results.push("MISS")

if(cache.length<capacity){
cache.push(block)
}else{
cache.shift()
cache.push(block)
}

}

renderCache(cache,hit?"HIT":"MISS")

await sleep(500)

}

buildTimeline(trace,results)

}



function generateRandom(){

const length=12
const maxBlock=6

let arr=[]

for(let i=0;i<length;i++){
arr.push(Math.floor(Math.random()*maxBlock)+1)
}

document.getElementById("trace").value=arr.join(" ")

}



function sleep(ms){
return new Promise(resolve=>setTimeout(resolve,ms))
}