'use strict';

const should = require('chai').should();
const request = require('request');
const requestURL = require('url');

const apiServer = require('../src/app.js');

const baseURL = 'http://localhost:8080';
const baseAPI = '/api';
const vitimasAPI = '/api/vitimas';

const VERDE = 'Verde';
const AMARELO = 'Amarelo';
const VERMELHO = 'Vermelho';
const PRETO = 'Preto';

describe('Vitimas', function() {

    describe ('API Pairwise', function() {
        var tests = [
            {exp: 1, deambulando: true, respiracao: 0, respiraViasAereas: true, reenchimentoCapilar: 2, pulsoRadial: true, cumpreOrdens: true},
            {exp: 2, deambulando: false, respiracao: 20, respiraViasAereas: false, reenchimentoCapilar: 3, pulsoRadial: false, cumpreOrdens: true},
            {exp: 3, deambulando: false, respiracao: 40, respiraViasAereas: true, reenchimentoCapilar: 3, pulsoRadial: true, cumpreOrdens: false},
            {exp: 4, deambulando: true, respiracao: 40, respiraViasAereas: false, reenchimentoCapilar: 2, pulsoRadial: false, cumpreOrdens: false},
            {exp: 5, deambulando: true, respiracao: 20, respiraViasAereas: true, reenchimentoCapilar: 2, pulsoRadial: false, cumpreOrdens: false},
            {exp: 6, deambulando: false, respiracao: 0, respiraViasAereas: false, reenchimentoCapilar: 2, pulsoRadial: false, cumpreOrdens: false},
            {exp: 7, deambulando: true, respiracao: 0, respiraViasAereas: false, reenchimentoCapilar: 3, pulsoRadial: true, cumpreOrdens: false},
            {exp: 8, deambulando: true, respiracao: 20, respiraViasAereas: false, reenchimentoCapilar: 2, pulsoRadial: true, cumpreOrdens: false},
            {exp: 9, deambulando: true, respiracao: 40, respiraViasAereas: false, reenchimentoCapilar: 2, pulsoRadial: true, cumpreOrdens: true}
        ];

        tests.forEach(function(data) {
            it('post experimento%d', function(done) {
                request({
                    url: requestURL.resolve(baseURL, vitimasAPI),
                    method: 'POST',
                    body: {
                        alias: 'experimento' + data.exp,
                        name: 'Teste 001',
                        timeCreated: '2016-09-16',
                        location: {
                            latitude: -23.0,
                            longitude: -45.0
                        },
                        deambulando: data.deambulando,
                        respiracao: data.respiracao,
                        respiraViasAereas: data.respiraViasAereas,
                        reenchimentoCapilar: data.reenchimentoCapilar,
                        pulsoRadial: data.pulsoRadial,
                        cumpreOrdens: data.cumpreOrdens
                    },
                    json: true
                }, function(error, response, body) {
                    should.not.exist(error);
                    should.exist(response);
                    response.statusCode.should.equal(200);
                    done();
                });
            });
        });

        context('Validação', () => {
            var tests = [
                {exp: 1, status: VERDE},
                {exp: 2, status: VERMELHO},
                {exp: 3, status: VERMELHO},
                {exp: 4, status: VERDE},
                {exp: 5, status: VERDE},
                {exp: 6, status: VERMELHO},
                {exp: 7, status: VERDE},
                {exp: 8, status: VERDE},
                {exp: 9, status: VERDE}
            ];

            tests.forEach(function(data) {
                it('get experimento' + data.exp, function(done) {
                    request({
                        url: requestURL.resolve(baseURL, vitimasAPI + '/experimento' + data.exp),
                        method: 'GET'
                    }, function(error, response, body) {
                        should.not.exist(error);
                        should.exist(response);
                        response.statusCode.should.equal(200);
                        should.exist(body);
                        var responseBody = JSON.parse(body);
                        console.log('Request: %s, expected: %s', responseBody['status'], data.status);
                        responseBody['status'].should.equal(data.status);
                        done();
                    });
                });
            });
        });

    });

});
