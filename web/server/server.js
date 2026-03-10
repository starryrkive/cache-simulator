const express = require("express")
const { exec } = require("child_process")
const path = require("path")

const app = express()

app.use(express.json())

// Serve frontend from web/
app.use(express.static(path.join(__dirname, "..")))

app.post("/simulate",(req,res)=>{

const capacity = req.body.capacity
const policy = req.body.policy
const trace = req.body.trace

// path to compiled C simulator
const simulator = path.join(__dirname, "..", "..", "cache_sim")

const command = `${simulator} ${policy} ${capacity} ${trace}`

console.log("Running:", command)

exec(command,(error,stdout,stderr)=>{

if(error){
console.log("Error:",error)
console.log(stderr)
res.send("Error running simulator")
return
}

res.send(stdout)

})

})

app.listen(3000,()=>{
console.log("Server running at http://localhost:3000")
})