import express from "express";
import readings from "./data/readings.js";

const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());
app.use(express.static("public"));


app.use(express.json());

app.get("/readings", (req, res) => {
    console.log("Reading all data");

    res
        .status(200)
        .json({
            "data": readings,
            "message": "Reading all data"
        })
})

app.get("/readings/:date", (req, res) => {
    const date = req.params.date;
    console.log("Reading all data", date);

    const reading = readings.find(r => r.date == date);

    res
        .status(200)
        .json({
            "data": readings,
            "message": "Reading all data"
        })
})

app.post("/readings", (req, res) => {
    console.log("New reading received:", req.body);
    const { humidity, temperature } = req.body || {};

    readings.push({
        humidity,
        temperature,
        date: new Date().toISOString()
    });

    res.status(201).json({
        message: "Reading added to data"
    });
});


app.use((req, res) => {
    res.json({
        "message": "Weather API"
    })
})

app.listen(PORT, () => {
    console.log("listening..",PORT);
})