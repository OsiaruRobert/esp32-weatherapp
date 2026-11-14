import express from "express";
import readings from "./data/readings.js";

const app = express();
const PORT = process.env.PORT || 3000;

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
    console.log("Reading all data");
    const readingPOST = req.body.readings;
    const date = Date.now();

    const reading = {
        ...readingPOST,
        date
    };

    console.log(reading);

    readings.push({
        reading
    });

    console.log(readings);
    
    res
        .status(201)
        .json({
            "message": "Reading added to data"
        })
})

app.use((req, res) => {
    res.json({
        "message": "Weather API"
    })
})

app.listen(PORT, () => {
    console.log("listening..");
})